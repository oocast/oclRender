#ifndef __CL_HELPER_H__
#define __CL_HELPER_H__

#include <CL/cl.h>
#include <CL/cl_intel.h>
#include "image.h"

extern cl_mem * importBuf;
extern cl_mem memObj[4];
extern cl_kernel kernel;
extern cl_command_queue cmdQueue;
extern cl_context context;
extern cl_int err;
extern cl_program program;
extern cl_platform_id platform_id;

void CLInit(char *, char *);
void CLRelease();
void CLReleaseCam();
void CLReadImageBuff(cl_mem &, PPMImage &);
void CLWriteImageBuff(cl_mem &, PPMImage &);

#endif /*__CL_HELPER_H__*/
