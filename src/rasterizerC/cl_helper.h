#ifndef __CL_HELPER_H__
#define __CL_HELPER_H__

#include "CL/cl.h"
#include "CL/cl_intel.h"
#include "oclrender.h"

extern cl_mem memObj[3];
extern cl_kernel kernel;
extern cl_command_queue cmd_queue;
extern cl_context context;
extern cl_int err;
extern cl_program program;

void init(char *, char *);
void release();
void image_modifyio(Scene &scene, PPMImage &bg);

#endif /*__CL_HELPER_H__*/
