#include <getopt.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>

#include <va/va.h>
#include <va/va_drmcommon.h>

#include "scene.h"
#include "va_display.h"
#include "cl_helper.h"
#include "va_helper.h"

#define BUFFER_NUM_DEFAULT 5
#define VIDEO_NODE_DEFAULT "/dev/video0"
#define WIDTH_DEFAULT 640
#define HEIGHT_DEFAULT 480

#define CHECK_VASTATUS(va_status,func)                                  \
  if (va_status != VA_STATUS_SUCCESS) {                                   \
    fprintf(stderr, "status = %d, %s: %s(line %d) failed, exit\n",va_status, __func__, func, __LINE__); \
    exit(1);                                                            \
    }

#define CHECK_CLSTATUS(status,func)                                  \
  if (status != CL_SUCCESS) {                                   \
    fprintf(stderr, "status = %d, %s: %s(line %d) failed, exit\n", status, __func__, func, __LINE__); \
    exit(1);                                                            \
    }

#define CHECK_V4L2ERROR(ret, STR)                               \
  if (ret){                             \
    fprintf(stderr, STR);            \
    perror(" ");                            \
    fprintf(stderr, "ret = %d, %s: %s(line %d) failed, exit\n", ret, __func__, STR, __LINE__);      \
    exit(1);                                  \
    }

cl_platform_id platform_ID;
v4l2_options vo;

int frame_count = 0;
cl_mem *importBuf = NULL;
int *importBuf_fd = NULL;
typedef cl_int(OCLGETMEMOBJECTFD)(cl_context, cl_mem, int *);
OCLGETMEMOBJECTFD *oclGetMemObjectFd = NULL;

int dev_fd;
uint64_t image_size;
unsigned int pitch;
VADisplay va_dpy;
cl_int cl_status;
VAStatus va_status;
VASurfaceID nv12_surface_id;
VAImage nv12_image;

const char short_options[] = "d:r:b:lh";

const struct option
long_options[] = 
{
    { "device", required_argument, NULL, 'd' },
    { "help", no_argument, NULL, 'h' },
    { "resolution", required_argument, NULL, 'r' },
    { "buffer_num", required_argument, NULL, 'b' },
    { "list", no_argument, NULL, 'l' },
    { 0, 0, 0, 0 }
};

void usage(FILE *fp, int argc, char **argv)
{
    fprintf(fp,
        "This example aims to demostrate the usage of DMABUF buffer sharing between v4l2 and Beignet.\n"
        "For more details, please read docs/howto/v4l2-buffer-sharing-howto.mdwn.\n"
        "Usage: %s [options]\n\n"
        "Options:\n"
        "-d | --device=<dev>  Specify device by <dev> instead of /dev/video0\n"
        "-h | --help          Print this message\n"
        "-r | --resolution=<width,height>    Set image resolution\n"
        "-b | --buffer_num=<num>  Set number of buffers\n"
        "-l | --list  List available resolution of format 'V4L2_PIX_FMT_YUYV'\n"
        "",
        argv[0]);
}

void list_resolution()
{
    int ret;
    struct v4l2_capability cap;
    struct v4l2_frmsizeenum frm_sz;

    dev_fd = open(vo.dev_name, O_RDWR | O_NONBLOCK, 0);
    if (dev_fd < 0) 
    {
        fprintf(stderr, "Can not open %s: %s\n",
            vo.dev_name, strerror(errno));
        exit(1);
    }

    memset(&cap, 0, sizeof(cap));
    ret = ioctl(dev_fd, VIDIOC_QUERYCAP, &cap);
    CHECK_V4L2ERROR(ret, "VIDIOC_QUERYCAP");

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        fprintf(stderr, "The device is not video capture device\n");
        exit(1);
    }
    if (!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        fprintf(stderr, "The device does not support streaming i/o\n");
        exit(1);
    }

    printf("Supported resolution under pixel format 'V4L2_PIX_FMT_YUYV':\n");
    frm_sz.pixel_format = V4L2_PIX_FMT_YUYV;
    frm_sz.index = 0;
    bool extra_info = true;
    while (ioctl(dev_fd, VIDIOC_ENUM_FRAMESIZES, &frm_sz) == 0) 
    {
        if (frm_sz.type == V4L2_FRMSIZE_TYPE_DISCRETE) 
        {
            if (extra_info)
            {
                printf("(width, height) = \n");
                extra_info = false;
            }
            printf("(%d, %d)", frm_sz.discrete.width, frm_sz.discrete.height);
            printf("\n");
        }
        else if (frm_sz.type == V4L2_FRMSIZE_TYPE_STEPWISE) 
        {
            printf("(width, height) from (%d, %d) to (%d, %d) with step (%d, %d)",
                frm_sz.stepwise.min_width,
                frm_sz.stepwise.min_height,
                frm_sz.stepwise.max_width,
                frm_sz.stepwise.max_height,
                frm_sz.stepwise.step_width,
                frm_sz.stepwise.step_height);
            continue;
        }
        frm_sz.index++;
    }

    ret = close(dev_fd);
    if (ret) 
    {
        fprintf(stderr, "Failed to close %s: %s\n",
            vo.dev_name, strerror(errno));
        exit(1);
    }
}

void AnalyseArgs(int argc, char *argv[])
{
    vo.dev_name = NULL;
    vo.width = 0;
    vo.height = 0;
    vo.spec_res = 0;
    vo.buffer_num = BUFFER_NUM_DEFAULT;
    vo.do_list = 0;

    int c, idx;
    for (;;) 
    {

        c = getopt_long(argc, argv,
            short_options, long_options, &idx);

        if (-1 == c)
            break;

        switch (c) 
        {
        case 0:
            break;

        case 'd':
            vo.dev_name = optarg;
            break;

        case '?':
        case 'h':
            usage(stdout, argc, argv);
            exit(0);

        case 'r':
            sscanf(optarg, "%d,%d", &vo.width, &vo.height);
            vo.spec_res = 1;
            break;

        case 'b':
            vo.buffer_num = strtoul(optarg, NULL, 0);
            break;

        case 'l':
            vo.do_list = 1;
            break;

        default:
            usage(stderr, argc, argv);
            exit(1);
        }
    }

    if (!vo.dev_name)
    {
        printf("Haven't specified device, use default device: %s\n",
            VIDEO_NODE_DEFAULT);
    }
    if (!vo.dev_name)
        vo.dev_name = VIDEO_NODE_DEFAULT;
    if (vo.do_list)
    {
        list_resolution();
        exit(0);
    }
    if (!vo.spec_res)
    {
        printf("Haven't specified resolution, use default resolution: (width,height) = (%d, %d)\n",
            WIDTH_DEFAULT, HEIGHT_DEFAULT);
        vo.width = WIDTH_DEFAULT;
        vo.height = HEIGHT_DEFAULT;
    }
    return;
}

void InitDevice()
{
    int ret;
    struct v4l2_capability cap;
    struct v4l2_format format;

    dev_fd = open(vo.dev_name, O_RDWR | O_NONBLOCK, 0);
    if (dev_fd < 0) 
    {
        fprintf(stderr, "Can not open %s: %s\n",
            vo.dev_name, strerror(errno));
        exit(1);
    }

    memset(&cap, 0, sizeof(cap));
    ret = ioctl(dev_fd, VIDIOC_QUERYCAP, &cap);
    CHECK_V4L2ERROR(ret, "VIDIOC_QUERYCAP");
    if (!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        fprintf(stderr, "The device does not support streaming i/o\n");
        exit(1);
    }

    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = vo.width;
    format.fmt.pix.height = vo.height;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    format.fmt.pix.field = V4L2_FIELD_ANY;

    ret = ioctl(dev_fd, VIDIOC_S_FMT, &format);
    CHECK_V4L2ERROR(ret, "VIDIOC_S_FMT");

    ret = ioctl(dev_fd, VIDIOC_G_FMT, &format);
    CHECK_V4L2ERROR(ret, "VIDIOC_G_FMT");
    if (format.fmt.pix.pixelformat != V4L2_PIX_FMT_YUYV)
    {
        fprintf(stderr, "V4L2_PIX_FMT_YUYV format is not supported by %s\n", vo.dev_name);
        exit(1);
    }
    if (format.fmt.pix.width != vo.width || format.fmt.pix.height != vo.height)
    {
        fprintf(stderr, "This resolution is not supported, please go through supported resolution by command './main -l'\n");
        exit(1);
    }
    printf("Input image format: (width, height) = (%u, %u), pixel format = %.4s\n",
        format.fmt.pix.width, format.fmt.pix.height, (char*)&format.fmt.pix.pixelformat);
    image_size = format.fmt.pix.sizeimage;
    pitch = format.fmt.pix.bytesperline;
}

void InitVaOcl(char * fileName, char * kernelName)
{
    int major_ver, minor_ver;

    printf("\n***********************libva info: ***********************\n");
    fflush(stdout);
    va_dpy = va_open_display();
    va_status = vaInitialize(va_dpy, &major_ver, &minor_ver);
    CHECK_VASTATUS(va_status, "vaInitialize");

    VASurfaceAttrib forcc;
    forcc.type = VASurfaceAttribPixelFormat;
    forcc.flags = VA_SURFACE_ATTRIB_SETTABLE;
    forcc.value.type = VAGenericValueTypeInteger;
    forcc.value.value.i = VA_FOURCC_NV12;
    va_status = vaCreateSurfaces(va_dpy, VA_RT_FORMAT_YUV420,
        vo.width, vo.height,
        &nv12_surface_id, 1, &forcc, 1);
    CHECK_VASTATUS(va_status, "vaCreateSurfaces");

    VAImageFormat image_fmt;
    image_fmt.fourcc = VA_FOURCC_NV12;
    image_fmt.byte_order = VA_LSB_FIRST;
    image_fmt.bits_per_pixel = 12;
    va_status = vaCreateImage(va_dpy, &image_fmt, vo.width, vo.height, &nv12_image);
    CHECK_VASTATUS(va_status, "vaCreateImage");

    //ocl initialization: basic & create kernel & get extension
    printf("\n***********************OpenCL info: ***********************\n");

    CLInit(fileName, kernelName);

    oclGetMemObjectFd = (OCLGETMEMOBJECTFD *)clGetExtensionFunctionAddressForPlatform(platform_ID, "clGetMemObjectFdIntel");

    if (!oclGetMemObjectFd)
    {
        fprintf(stderr, "Failed to get extension clGetMemObjectFdIntel\n");
        exit(1);
    }
    printf("\n***********************************************************\n");
}

void CreateDmasharingBuffers()
{
    if (importBuf_fd == NULL)
        importBuf_fd = new int[vo.buffer_num];
    if (importBuf == NULL)
        importBuf = new cl_mem[vo.buffer_num];

    for (unsigned int i = 0; i < vo.buffer_num; ++i)
    {
        importBuf[i] = clCreateBuffer(context, CL_MEM_READ_WRITE, image_size, NULL, &cl_status);
        CHECK_CLSTATUS(cl_status, "clCreateBuffer");

        //get cl buffer object's fd
        cl_status = oclGetMemObjectFd(context, importBuf[i], &importBuf_fd[i]);
        CHECK_CLSTATUS(cl_status, "clGetMemObjectFdIntel");
    }
}

void InitDmabuf(void)
{
    int ret;
    struct v4l2_requestbuffers reqbuf;

    memset(&reqbuf, 0, sizeof(reqbuf));
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_DMABUF;
    reqbuf.count = vo.buffer_num;

    ret = ioctl(dev_fd, VIDIOC_REQBUFS, &reqbuf);
    if (ret == -1 && errno == EINVAL)
    {
        fprintf(stderr, "Video capturing or DMABUF streaming is not supported\n");
        exit(1);
    }
    else
        CHECK_V4L2ERROR(ret, "VIDIOC_REQBUFS");

    CreateDmasharingBuffers();
    printf("Succeed to create %d dma buffers \n", vo.buffer_num);

}

void ReleaseVaOcl()
{
    va_status = vaDestroySurfaces(va_dpy, &nv12_surface_id, 1);
    CHECK_VASTATUS(va_status, "vaDestroySurfaces");
    va_status = vaDestroyImage(va_dpy, nv12_image.image_id);
    CHECK_VASTATUS(va_status, "vaDestroyImage");
    va_status = vaTerminate(va_dpy);
    CHECK_VASTATUS(va_status, "vaTerminate");
    va_close_display(va_dpy);

    int ret;
    for (unsigned int i = 0; i < vo.buffer_num; ++i) {
        ret = close(importBuf_fd[i]);
        if (ret)
            fprintf(stderr, "Failed to close importBuf[%u]'s fd: %s\n", i, strerror(errno));
        cl_status = clReleaseMemObject(importBuf[i]);
        CHECK_CLSTATUS(cl_status, "clReleaseMemObject");
    }
    CLReleaseCam();
}

void ReleaseDevice(void)
{
    delete[] importBuf_fd;
    delete[] importBuf;
    int ret = close(dev_fd);
    if (ret) 
    {
        fprintf(stderr, "Failed to close %s: %s\n",
            vo.dev_name, strerror(errno));
        exit(1);
    }
}

void StartCapturing()
{
    int ret;
    for (unsigned int i = 0; i < vo.buffer_num; ++i) 
    {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_DMABUF;
        buf.index = i;
        buf.m.fd = importBuf_fd[i];
        ret = ioctl(dev_fd, VIDIOC_QBUF, &buf);
        CHECK_V4L2ERROR(ret, "VIDIOC_QBUF");
    }

    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(dev_fd, VIDIOC_STREAMON, &type);
    CHECK_V4L2ERROR(ret, "VIDIOC_STREAMON");
}

void ProcessFrame(int index, ScenePtr & scene)
{
    //process importBuf[index] by ocl
    scene->Draw(vo.height, vo.width, index, 6);
}

void ShowFrame(int index)
{
    //create corresponding VASurface
    VASurfaceID yuy2_surface_id;
    VASurfaceAttrib sa[2];
    sa[0].type = VASurfaceAttribMemoryType;
    sa[0].flags = VA_SURFACE_ATTRIB_SETTABLE;
    sa[0].value.type = VAGenericValueTypeInteger;
    sa[0].value.value.i = VA_SURFACE_ATTRIB_MEM_TYPE_DRM_PRIME;
    sa[1].type = VASurfaceAttribExternalBufferDescriptor;
    sa[1].flags = VA_SURFACE_ATTRIB_SETTABLE;
    sa[1].value.type = VAGenericValueTypePointer;
    VASurfaceAttribExternalBuffers sa_eb;
    sa_eb.pixel_format = VA_FOURCC_YUY2;
    sa_eb.width = vo.width;
    sa_eb.height = vo.height;
    sa_eb.data_size = image_size;
    sa_eb.num_planes = 1;
    sa_eb.pitches[0] = pitch;
    sa_eb.offsets[0] = 0;
    sa_eb.num_buffers = 1;
    sa_eb.buffers = new unsigned long[sa_eb.num_buffers];
    sa_eb.buffers[0] = importBuf_fd[index];
    sa_eb.flags = 0;
    sa[1].value.value.p = &sa_eb;
    va_status = vaCreateSurfaces(va_dpy, VA_RT_FORMAT_YUV422,
        vo.width, vo.height,
        &yuy2_surface_id, 1, sa, 2);
    CHECK_VASTATUS(va_status, "vaCreateSurfaces");

    //convert to NV12 format
    va_status = vaGetImage(va_dpy, yuy2_surface_id, 0, 0,
        vo.width, vo.height, nv12_image.image_id);
    CHECK_VASTATUS(va_status, "vaGetImage");
    va_status = vaPutImage(va_dpy, nv12_surface_id, nv12_image.image_id,
        0, 0, vo.width, vo.height, 0, 0,
        vo.width, vo.height);
    CHECK_VASTATUS(va_status, "vaPutImage");

    //show by vaPutsurface
    VARectangle src_rect, dst_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = vo.width;
    src_rect.height = vo.height;
    dst_rect = src_rect;
    va_status = va_put_surface(va_dpy, nv12_surface_id, &src_rect, &dst_rect);
    CHECK_VASTATUS(va_status, "vaPutSurface");

    vaDestroySurfaces(va_dpy, &yuy2_surface_id, 1);
    CHECK_VASTATUS(va_status, "vaDestroySurfaces");
    delete[] sa_eb.buffers;
    return;
}

void MainLoop(ScenePtr & scene)
{
    int ret;
    struct v4l2_buffer buf;
    int index;
    unsigned int st=0, nst=0;

    while (frame_count < 1000) 
    {
        frame_count++;
        printf("******************Frame %d\n", frame_count);
        fd_set fds;
        struct timeval tv;
        int r;

        FD_ZERO(&fds);
        FD_SET(dev_fd, &fds);

        /* Timeout. */
        tv.tv_sec = 2;
        tv.tv_usec = 0;


        r = select(dev_fd + 1, &fds, NULL, NULL, &tv);

        if (-1 == r) 
        {
            if (EINTR == errno)
                continue;
            perror("select");
        }

        if (r == 0)
        {
            fprintf(stderr, "Select timeout\n");
            exit(1);
        }

        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_DMABUF;
        ret = ioctl(dev_fd, VIDIOC_DQBUF, &buf);
        CHECK_V4L2ERROR(ret, "VIDIOC_DQBUF");
        index = buf.index;

        timespec StartTime, EndTime;
        //process by ocl and show on screen by libva
        clock_gettime(CLOCK_REALTIME, &StartTime);
        ProcessFrame(index, scene);
        clock_gettime(CLOCK_REALTIME, &EndTime);
        ShowFrame(index);

        //Then queue this buffer(buf.index) by QBUF
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_DMABUF;
        buf.m.fd = importBuf_fd[index];
        buf.index = index;

        ret = ioctl(dev_fd, VIDIOC_QBUF, &buf);
        CHECK_V4L2ERROR(ret, "VIDIOC_QBUF");
        unsigned int s, ns;
        s=EndTime.tv_sec-StartTime.tv_sec;
        ns=EndTime.tv_nsec-StartTime.tv_nsec;
        if (ns<0)
        {
            ns+=1000000000;
            s--;
        }
        st+=s;
        nst+=ns;
        if (nst>1000000000)
        {
            nst-=1000000000;
            st++;
        }
        std::cout<<"Total running time "<<s<<" s "<<ns<<" ns."<<std::endl;
        std::cout<<"Total running time "<<st<<" s "<<nst<<" ns."<<std::endl;
    }
    std::cout<<"Total running time "<<st<<" s "<<nst<<" ns."<<std::endl;
}

void StopCapturing()
{
    int ret;
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(dev_fd, VIDIOC_STREAMOFF, &type);
    CHECK_V4L2ERROR(ret, "VIDIOC_STREAMOFF");
}
