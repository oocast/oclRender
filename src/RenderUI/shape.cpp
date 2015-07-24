#include <random>
#include <cmath>

#include "shape.h"
#include "cl_helper.h"

#include <iostream>
/* for packed rgb/yuv
const int HSIZE=16;
const int WSIZE=32;
*/
const int HSIZE=32;
const int WSIZE=16;

//cl_mem * importBuf;
cl_mem memObj[4];
cl_kernel kernel;
cl_command_queue cmdQueue;
cl_context context;
cl_int err;
cl_program program;

Shape::
Shape(const Color * colorInputPointer, bool positive):
positive(positive)
{
    if (colorInputPointer != nullptr)
        shapeColor = *colorInputPointer;
    else
        shapeColor = Color();
}

/*
void Shape::
draw(PPMImage &image, int superSampling)
{
    if (!bound.overlaps(image.bounds())) {
        return;
    }
    float r = (float)image.resolution;
    std::vector<Vector> jitter(superSampling*superSampling, Vector());
    std::default_random_engine gen;
    std::uniform_real_distribution<float> distri(0.0, 1.0);
    for (int x = 0; x < superSampling; x++) {
        for (int y = 0; y < superSampling; y++) {
            jitter[x * superSampling + y] = Vector( ((float)x + distri(gen)) / superSampling / r,
                ((float)y + distri(gen)) / superSampling / r);
        }
    }
    size_t jitterSize = jitter.size();
    int l_x, l_y, h_x, h_y;
    l_x = (int)fmax(bound.low.x * r, 0);
    l_y = (int)fmax(bound.low.y * r, 0);
    h_x = (int)fmin(bound.high.x * r, r-1);
    h_y = (int)fmin(bound.high.y * r, r-1);
    for (int y = l_y; y <= h_y; y++) {
        int x = l_x;
        while (x <= h_x) {
            Vector corner = Vector((float)x / r, (float)y / r);
            float b = signed_distance_bound(corner);
            float pixel_diameter = sqrt(2.0) / r;
            if (b >= pixel_diameter) {
                int steps = (int)(r * (b - pixel_diameter + 1.0 / r));
                int xend = (int)fmin(x + steps, h_x + 1);
                for (int x_ = x; x_ < xend; x_++) {
                    //image.pixels[y * image.resolution + x_].draw(color);
                    draw_Color(&image.pixels[y * image.resolution + x_], &color);
                }
                x += steps;
            }
            else if (b <= -pixel_diameter) {
                int steps = (int)(r * (-b - pixel_diameter + 1.0 / r));
                x += steps;
            }
            else {
                float coverage = 0.0;
                for (size_t i = 0; i < jitterSize; i++) {
                    if (contains(corner + jitter[i])) {
                        coverage += 1.0;
                    }
                }
                Color fainted = faint_Color(&color, coverage / jitterSize);
                draw_Color(&image.pixels[y * image.resolution + x], &fainted);
                x++;
            }
        }
    }
}
*/

void Shape::
DrawRGB(PPMImage & image, int superSampling)
{
    if (!bound.Overlaps(image.Bounds()))
        return;
    float r = (float)image.width;
    std::vector<Vector> jitter(superSampling*superSampling, Vector());
    std::default_random_engine gen;
    std::uniform_real_distribution<float> distri(0.0, 1.0);
    for (int x = 0; x < superSampling; x++)
        for (int y = 0; y < superSampling; y++)
            jitter[x * superSampling + y] = Vector(((float)x + distri(gen)) / superSampling / r,
                ((float)y + distri(gen)) / superSampling / r);

    size_t jitterSize = jitter.size();

    int w=image.width;
    int h=image.height;
    CalculateBound((float)h/w);
///*
    std::vector<float> fv;
    std::vector<int> iv;
    for (int i=0; i<4; i++) iv.push_back(1);
    GetParameters(fv, iv);

    int ib=(int) (bound.low.y*w);
    int jb=(int) (bound.low.x*w);
    int ie=ceil(bound.high.y*w);
    int je=ceil(bound.high.x*w);
    if (ie<=0) return;
    if (je<=0) return;
    if (ib<0) ib=0;
    if (jb<0) jb=0;

    memObj[1]=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, fv.size()*sizeof(float), &fv[0], NULL);
    memObj[2]=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, 2*jitterSize*sizeof(float), &jitter[0], NULL);
    memObj[3]=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, iv.size()*sizeof(int), &iv[0], NULL);

    err=clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &memObj[0]);
    err=clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *) &memObj[1]);
    err=clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *) &memObj[2]);
    err=clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *) &memObj[3]);
    err=clSetKernelArg(kernel, 4, sizeof(int), (void *) &jitterSize);
    err=clSetKernelArg(kernel, 5, sizeof(int), (void *) &w);
    err=clSetKernelArg(kernel, 6, sizeof(int), (void *) &h);
    err=clSetKernelArg(kernel, 7, sizeof(float), (void *) &shapeColor.rgb[0]);
    err=clSetKernelArg(kernel, 8, sizeof(float), (void *) &shapeColor.rgb[1]);
    err=clSetKernelArg(kernel, 9, sizeof(float), (void *) &shapeColor.rgb[2]);
    err=clSetKernelArg(kernel, 10, sizeof(float), (void *) &shapeColor.a);
    err=clSetKernelArg(kernel, 11, sizeof(int), (void *) &ib);
    err=clSetKernelArg(kernel, 12, sizeof(int), (void *) &jb);

    size_t globals[2];
    size_t locals[2];

    globals[0]=ceil((ie-ib)*1.0f/HSIZE)*HSIZE;
    globals[1]=ceil((je-jb)*1.0f/WSIZE)*WSIZE;
    locals[0]=HSIZE;
    locals[1]=WSIZE;
    
    err=clEnqueueNDRangeKernel(cmdQueue, kernel, 2, NULL, globals, locals, 0, NULL, NULL);

    err=clReleaseMemObject(memObj[1]);
    err=clReleaseMemObject(memObj[2]);
    err=clReleaseMemObject(memObj[3]);
    err=clFinish(cmdQueue);
//*/
/*
    int l_x, l_y, h_x, h_y;
    l_x = (int)fmax(bound.low.x * r, 0);
    l_y = (int)fmax(bound.low.y * r, 0);
    h_x = (int)fmin(bound.high.x * r, w - 1);
    h_y = (int)fmin(bound.high.y * r, h - 1);
    for (int y = l_y; y <= h_y; y++) {
        int x = l_x;
        while (x <= h_x) {
            Vector corner = Vector((float)x / r, (float)y / r);
            float coverage = 0.0;
            for (size_t i = 0; i < jitterSize; i++) {
                if (Contains(corner + jitter[i])) {
                    coverage += 1.0;
                }
            }
            Color fainted = FaintColor(&shapeColor, coverage / jitterSize);
            DrawColor(&image.pixels[y * image.width + x], &fainted);
            x++;
        }
    }
*/
}

void Shape::
DrawYUYV(PPMImage & image, int superSampling)
{
    if (!bound.Overlaps(image.Bounds()))
        return;
    float r = (float)image.width;
    std::vector<Vector> jitter(superSampling*superSampling, Vector());
    std::default_random_engine gen;
    std::uniform_real_distribution<float> distri(0.0, 1.0);
    for (int x = 0; x < superSampling; x++)
        for (int y = 0; y < superSampling; y++)
            jitter[x * superSampling + y] = Vector(((float)x + distri(gen)) / superSampling / r,
                ((float)y + distri(gen)) / superSampling / r);

    size_t jitterSize = jitter.size();

    int w=image.width;
    int h=image.height;
    CalculateBound((float)h/w);

    std::vector<float> fv;
    std::vector<int> iv;
    for (int i=0; i<4; i++) iv.push_back(1);
    GetParameters(fv, iv);

    int ib=(int) (bound.low.y*w);
    int jb=(int) (bound.low.x*w/2);
    int ie=ceil(bound.high.y*w);
    int je=ceil(bound.high.x*w/2);
    if (ie<=0) return;
    if (je<=0) return;
    if (ib<0) ib=0;
    if (jb<0) jb=0;

    memObj[1]=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, fv.size()*sizeof(float), &fv[0], NULL);
    memObj[2]=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, 2*jitterSize*sizeof(float), &jitter[0], NULL);
    memObj[3]=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, iv.size()*sizeof(int), &iv[0], NULL);

    err=clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &memObj[0]);
    err=clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *) &memObj[1]);
    err=clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *) &memObj[2]);
    err=clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *) &memObj[3]);
    err=clSetKernelArg(kernel, 4, sizeof(int), (void *) &jitterSize);
    err=clSetKernelArg(kernel, 5, sizeof(int), (void *) &w);
    err=clSetKernelArg(kernel, 6, sizeof(int), (void *) &h);
    err=clSetKernelArg(kernel, 7, sizeof(float), (void *) &shapeColor.rgb[0]);
    err=clSetKernelArg(kernel, 8, sizeof(float), (void *) &shapeColor.rgb[1]);
    err=clSetKernelArg(kernel, 9, sizeof(float), (void *) &shapeColor.rgb[2]);
    //err=clSetKernelArg(kernel, 10, sizeof(float), (void *) &shapeColor.a);
    err=clSetKernelArg(kernel, 10, sizeof(int), (void *) &ib);
    err=clSetKernelArg(kernel, 11, sizeof(int), (void *) &jb);

    size_t globals[2];
    size_t locals[2];

    globals[0]=ceil((ie-ib)*1.0f/HSIZE)*HSIZE;
    globals[1]=ceil((je-jb)*1.0f/WSIZE)*WSIZE;
    locals[0]=HSIZE;
    locals[1]=WSIZE;
    
    err=clEnqueueNDRangeKernel(cmdQueue, kernel, 2, NULL, globals, locals, 0, NULL, NULL);

    err=clReleaseMemObject(memObj[1]);
    err=clReleaseMemObject(memObj[2]);
    err=clReleaseMemObject(memObj[3]);
    err=clFinish(cmdQueue);
}

void Shape::
Draw(PPMImage & image, int superSampling)
{
    DrawRGB(image, superSampling);
}

void Shape::
Draw(unsigned int height, unsigned int width, int index, int superSampling)
{
    float r = (float)width;
    std::vector<Vector> jitter(superSampling*superSampling, Vector());
    std::default_random_engine gen;
    std::uniform_real_distribution<float> distri(0.0, 1.0);
    for (int x = 0; x < superSampling; x++)
        for (int y = 0; y < superSampling; y++)
            jitter[x * superSampling + y] = Vector(((float)x + distri(gen)) / superSampling / r,
                ((float)y + distri(gen)) / superSampling / r);

    size_t jitterSize = jitter.size();

    int w = width;
    int h = height;
    CalculateBound((float)h / w);
    if (!bound.Overlaps(AABox(Vector(), Vector(1, height*1.0f/width))))
        return;

    std::vector<float> fv;
    std::vector<int> iv;
    for (int i = 0; i<4; i++) iv.push_back(1);
    GetParameters(fv, iv);

    int ib = (int)(bound.low.y*w);
    int jb = (int)(bound.low.x*w / 2);
    int ie = ceil(bound.high.y*w);
    int je = ceil(bound.high.x*w / 2);
    //if (ie <= 0) return;
    //if (je <= 0) return;
    if (ib<0) ib = 0;
    if (jb<0) jb = 0;
    if (ie>=h)
    {
        ie=h;
        ib=ib/HSIZE*HSIZE;
    }
    if (je>=w/2)
    {
        je=w/2;
        jb=jb/WSIZE*WSIZE;
    }
    unsigned char Y=floor(shapeColor.rgb[0]*256);
    unsigned char U=floor(shapeColor.rgb[1]*256);
    unsigned char V=floor(shapeColor.rgb[2]*256);
    int strucSize=iv.size()/2;
    int paraSize=fv.size()/4;

    memObj[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, iv.size()*sizeof(int), &iv[0], NULL);
    memObj[2] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, fv.size()*sizeof(float), &fv[0], NULL);
    memObj[3] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 2 * jitterSize*sizeof(float), &jitter[0], NULL);

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &importBuf[index]);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *) &memObj[1]);
    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *) &memObj[2]);
    err = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *) &memObj[3]);
//    err = clSetKernelArg(kernel, 4, sizeof(int), (void *) &jitterSize);
    err = clSetKernelArg(kernel, 4, sizeof(int), (void *) &w);
    err = clSetKernelArg(kernel, 5, sizeof(int), (void *) &h);
    err = clSetKernelArg(kernel, 6, sizeof(unsigned char), (void *) &Y);
    err = clSetKernelArg(kernel, 7, sizeof(unsigned char), (void *) &U);
    err = clSetKernelArg(kernel, 8, sizeof(unsigned char), (void *) &V);
    err = clSetKernelArg(kernel, 9, sizeof(float), (void *) &shapeColor.a);
    err = clSetKernelArg(kernel, 10, sizeof(int), (void *) &ib);
    err = clSetKernelArg(kernel, 11, sizeof(int), (void *) &jb);
    err = clSetKernelArg(kernel, 12, iv.size()*sizeof(int), NULL);
    err = clSetKernelArg(kernel, 13, fv.size()*sizeof(float), NULL);
    err = clSetKernelArg(kernel, 14, sizeof(int), (void *) &strucSize);
    err = clSetKernelArg(kernel, 15, sizeof(int), (void *) &paraSize);

    size_t globals[2];
    size_t locals[2];

    globals[0] = ceil((ie - ib)*1.0f / HSIZE)*HSIZE;
    globals[1] = ceil((je - jb)*1.0f / WSIZE)*WSIZE;
    locals[0] = HSIZE;
    locals[1] = WSIZE;

    err = clEnqueueNDRangeKernel(cmdQueue, kernel, 2, NULL, globals, locals, 0, NULL, NULL);

    err = clReleaseMemObject(memObj[1]);
    err = clReleaseMemObject(memObj[2]);
    err = clReleaseMemObject(memObj[3]);
    err = clFinish(cmdQueue);
}
