#include <random>
#include <cmath>

#include "shape.h"
#include "cl_helper.h"

cl_mem memObj[3];
cl_kernel kernel;
cl_command_queue cmdQueue;
cl_context context;
cl_int err;
cl_program program;

Shape::
Shape(const Color * oColor)
{
    if (oColor != nullptr)
        shapeColor = *oColor;
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
Draw(PPMImage & image, int superSampling)
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

    std::vector<float> fv;
    ShapeType shapeType;
    GetParameters(fv, shapeType);
    int identifier=0;
    if (shapeType==CONVEXPOLY) 
        identifier=fv.size()/3;

    size_t jitterSize = jitter.size();
///*
    int w=image.width;
    int h=image.height;

    memObj[1]=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, fv.size()*sizeof(float), &fv[0], NULL);
    memObj[2]=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, 2*jitterSize*sizeof(float), &jitter[0], NULL);

    err=clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &memObj[0]);
    err=clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *) &memObj[1]);
    err=clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *) &memObj[2]);
    err=clSetKernelArg(kernel, 3, sizeof(int), (void *) &jitterSize);
    err=clSetKernelArg(kernel, 4, sizeof(int), (void *) &w);
    err=clSetKernelArg(kernel, 5, sizeof(int), (void *) &h);
    err=clSetKernelArg(kernel, 6, sizeof(float), (void *) &shapeColor.rgb[0]);
    err=clSetKernelArg(kernel, 7, sizeof(float), (void *) &shapeColor.rgb[1]);
    err=clSetKernelArg(kernel, 8, sizeof(float), (void *) &shapeColor.rgb[2]);
    err=clSetKernelArg(kernel, 9, sizeof(float), (void *) &shapeColor.a);
    err=clSetKernelArg(kernel, 10, sizeof(int), (void *) &identifier);
    size_t globals[1];
    size_t locals[1];
    globals[0]=h;
    locals[0]=h;
    
    err=clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, globals, locals, 0, NULL, NULL);

    err=clReleaseMemObject(memObj[1]);
    err=clReleaseMemObject(memObj[2]);
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
                if (contains(corner + jitter[i])) {
                    coverage += 1.0;
                }
            }
            Color fainted = faint_Color(&color, coverage / jitterSize);
            draw_Color(&image.pixels[y * image.width + x], &fainted);
            x++;
        }
    }
*/
}
