#ifndef __VA_HELPER_H__
#define __VA_HELPER_H__

#include "scene.h"

extern struct v4l2_options
{
    const char *dev_name;
    unsigned int width, height;
    unsigned int spec_res;
    unsigned int buffer_num;
    unsigned int do_list;
} vo;

typedef Scene * ScenePtr;

void AnalyseArgs(int, char **);
void InitDevice();
void InitVaOcl(char *, char *);
void InitDmabuf();
void ReleaseVaOcl();
void ReleaseDevice();
void StartCapturing();
void MainLoop(ScenePtr &);
void StopCapturing();

#endif /*__VA_HELPER_H__*/
