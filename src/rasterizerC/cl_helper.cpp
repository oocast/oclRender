#include <iostream>

#include"cl_helper.h"

    cl_platform_id platform_id=NULL;

void CLInit(char * fileName, char * kernelName)
{
    std::ifstream iFile;
    iFile.open(fileName);
    iFile.seekg(0, std::ios::end);
    int length=iFile.tellg();
    iFile.seekg(0, std::ios::beg);
    char * source=new char [length];
    iFile.read(source, length);
    iFile.close();


    cl_device_id device_id=NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    err=clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    err=clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

    context=clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);    
    cmdQueue=clCreateCommandQueue(context, device_id, 0, &err);

    program=clCreateProgramWithSource(context, 1, (const char **) & source, (const size_t *) & length, &err);

    err=clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    kernel=clCreateKernel(program, kernelName, &err);
    free(source);
}

void CLRelease()
{
    err=clFlush(cmdQueue);
    err=clFinish(cmdQueue);
    err=clReleaseKernel(kernel);
    err=clReleaseProgram(program);
    err=clReleaseMemObject(memObj[0]);
    err=clReleaseCommandQueue(cmdQueue);
    err=clReleaseContext(context);
}

void CLReleaseCam()
{
    err = clFlush(cmdQueue);
    err = clFinish(cmdQueue);
    err = clReleaseKernel(kernel);
    err = clReleaseProgram(program);
    
    // don't bother release it because it is not used
    //err = clReleaseMemObject(memObj[0]);
    
    err = clReleaseCommandQueue(cmdQueue);
    err = clReleaseContext(context);
}

void CLReadImageBuff(cl_mem & memObj, PPMImage & image)
{
    int size = image.pixels.size()*sizeof(Color);
    memObj = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size, &image.pixels[0], NULL);
}

void CLWriteImageBuff(cl_mem & memObj, PPMImage & image)
{
    int size = image.pixels.size()*sizeof(Color);
    err = clEnqueueReadBuffer(cmdQueue, memObj, CL_TRUE, 0, size, &image.pixels[0], 0, NULL, NULL);
}


