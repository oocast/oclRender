#include <stdio.h>
#include <stdlib.h>
#include"cl_helper.h"

void init(char * fileName, char * kernelName)
{
	FILE * fp;
	fp=fopen(fileName, "r");
	//if (!fp) std::cout<<"asas"<<std::endl;
	unsigned char * source=(unsigned char *)malloc(0x100000);
	size_t srcSize=fread(source, 1, 0x100000, fp);
	fclose(fp);

	cl_platform_id platform_id=NULL;
	cl_device_id device_id=NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	err=clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	err=clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	context=clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);	
	cmd_queue=clCreateCommandQueue(context, device_id, 0, &err);

	program=clCreateProgramWithSource(context, 1, (const char **) & source, (const size_t *) & srcSize, &err);

	err=clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel=clCreateKernel(program, kernelName, &err);
}

void release()
{
	err=clFlush(cmd_queue);
	err=clFinish(cmd_queue);
	err=clReleaseKernel(kernel);
	err=clReleaseProgram(program);
	err=clReleaseMemObject(memObj[0]);
	err=clReleaseCommandQueue(cmd_queue);
	err=clReleaseContext(context);
}


