#include <random>
#include <vector>
#include <cmath>
#include "color.h"
#include "geometry.h"
#include "shape.h"
#include "cl_helper.h"
#include <iostream>

cl_mem memObj[3];
cl_kernel kernel;
cl_command_queue cmd_queue;
cl_context context;
cl_int err;
cl_program program;

Shape::
Shape(const Color *o_color)
{
  if (o_color != nullptr) {
    color = *o_color;
  }
	else {
		color = Color();
	}
}

/*
void Shape::
draw(PPMImage &image, int super_sampling)
{
  if (!bound.overlaps(image.bounds())) {
    return;
  }
  float r = (float)image.resolution;
  std::vector<Vector> jitter(super_sampling*super_sampling, Vector());
  std::default_random_engine gen;
  std::uniform_real_distribution<float> distri(0.0, 1.0);
  for (int x = 0; x < super_sampling; x++) {
    for (int y = 0; y < super_sampling; y++) {
      jitter[x * super_sampling + y] = Vector( ((float)x + distri(gen)) / super_sampling / r,
        ((float)y + distri(gen)) / super_sampling / r);
    }
  }
  size_t lj = jitter.size();
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
        for (size_t i = 0; i < lj; i++) {
          if (contains(corner + jitter[i])) {
            coverage += 1.0;
          }
        }
				Color fainted = faint_Color(&color, coverage / lj);
        draw_Color(&image.pixels[y * image.resolution + x], &fainted);
        x++;
      }
    }
  }
}
*/

void Shape::
draw(PPMImage &image, int super_sampling)
{
	if (!bound.overlaps(image.bounds())) {
		return;
	}
	float r = (float)image.width;
	//float h = (float)image.height;
	//float r = fmin(w, h);
	//float r=w;
	std::vector<Vector> jitter(super_sampling*super_sampling, Vector());
	std::default_random_engine gen;
	std::uniform_real_distribution<float> distri(0.0, 1.0);
	for (int x = 0; x < super_sampling; x++) {
		for (int y = 0; y < super_sampling; y++) {
			jitter[x * super_sampling + y] = Vector(((float)x + distri(gen)) / super_sampling / r,
				((float)y + distri(gen)) / super_sampling / r);
		}
	}

	std::vector<float> fv;
	ShapeType shape;
	get_parameters(fv, &shape);
	int identifier=0;
	if (shape==CONVEXPOLY) identifier=fv.size()/3;
	//std::cout<<identifier<<std::endl;

	size_t lj = jitter.size();
///*
	int wi=image.width;
	int he=image.height;
	//int size=image.width*image.height*sizeof(Color);

	memObj[1]=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, fv.size()*sizeof(float), &fv[0], NULL);
	memObj[2]=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, 2*lj*sizeof(float), &jitter[0], NULL);

	err=clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &memObj[0]);
	err=clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *) &memObj[1]);
	err=clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *) &memObj[2]);
	err=clSetKernelArg(kernel, 3, sizeof(int), (void *) &lj);
	err=clSetKernelArg(kernel, 4, sizeof(int), (void *) &wi);
	err=clSetKernelArg(kernel, 5, sizeof(int), (void *) &he);
	err=clSetKernelArg(kernel, 6, sizeof(float), (void *) &color.rgb[0]);
	err=clSetKernelArg(kernel, 7, sizeof(float), (void *) &color.rgb[1]);
	err=clSetKernelArg(kernel, 8, sizeof(float), (void *) &color.rgb[2]);
	//err=clSetKernelArg(kernel, 9, sizeof(cl_mem), (void *) &memObj[3]);
	err=clSetKernelArg(kernel, 9, sizeof(int), (void *) &identifier);
	size_t globals[1];
	size_t locals[1];
	globals[0]=he;
	locals[0]=he;
	
	err=clEnqueueNDRangeKernel(cmd_queue, kernel, 1, NULL, globals, locals, 0, NULL, NULL);

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
			for (size_t i = 0; i < lj; i++) {
				if (contains(corner + jitter[i])) {
					coverage += 1.0;
				}
			}
			Color fainted = faint_Color(&color, coverage / lj);
			draw_Color(&image.pixels[y * image.width + x], &fainted);
			x++;
		}
	}
*/


}
