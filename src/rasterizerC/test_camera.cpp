/*
** Copyright (c) 2012, 2015 Intel Corporation. All Rights Reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sub license, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice (including the
** next paragraph) shall be included in all copies or substantial portions
** of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
** OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
** IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
** ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**/

#include "oclrender.h"
#include "interface.h"
#include "va_helper.h"

const float PI=3.1415927f;
char fileName[]="shapedraw2_yuyv.cl";
char kernelName[]="ShapeDraw";

void 
TestCameraRender()
{
    Scene scene;

    Color color;
    InitColor(&color, 255/256.0, 255/256.0, 255/256.0, 1);
    ToYUV(&color);
    //ConvexPoly *cp = new ConvexPoly(Rectangle(Vector(0.04, 0.04), Vector(0.06, 0.06)));
    std::vector<Vector> vv1, vv2;
    std::shared_ptr<Shape> sp;

    vv1.push_back(Vector(0, 0.02));
    vv1.push_back(Vector(-0.02, 0));
    vv1.push_back(Vector(0, -0.02));
    vv1.push_back(Vector(0.02, 0));
    vv2.push_back(Vector(0.3, 0.3));
    vv2.push_back(Vector(0.7, 0.3));
    vv2.push_back(Vector(0.3, 0.7));
    //vv2.push_back(Vector(0.3, 0.7));

    ConvexPoly * cp=new ConvexPoly(vv1, NULL, 1);

    Intersection * ip=new Intersection(NULL, 1);

    sp=(std::shared_ptr<Shape>) cp;
    ip->AddElement(sp);

    Union * up1=new Union(&color, 1);

    sp=(std::shared_ptr<Shape>) ip;
    up1->AddElement(sp);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            scene.Add(up1->TransformPointer(Rotate(PI/2/25*(i*5+j)))->TransformPointer(Translate(0.05 + i * 0.2, 0.05 + j * 0.2)));

    sp=(std::shared_ptr<Shape>)BezierCurve(vv2, 0.02);

    Union * up2=new Union(&color, 1);

    up2->AddElement(sp);

    sp=(std::shared_ptr<CSG>)EllipseRing(1, 2, 0, -0.8, -1.4, 0.3, 0.02);
    
    Union * up3=new Union(&color, 1);
    
    up2->AddElement(sp);

    scene.Add(up2);
    scene.Add(up3);

    ScenePtr scenePtr=&scene;

    StartCapturing();
    MainLoop(scenePtr);
    StopCapturing();
}



int main(int argc, char *argv[])
{
    AnalyseArgs(argc, argv);

    InitDevice();
    InitVaOcl(fileName, kernelName);
    InitDmabuf();

    TestCameraRender();

    ReleaseVaOcl();
    ReleaseDevice();

    return 0;
}
