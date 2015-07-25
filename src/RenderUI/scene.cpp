#include "scene.h"
#include "cl_helper.h"

#include<time.h>
#include<sys/time.h>
#include<iostream>

void Scene::
Add(Shape * node)
{
    nodes.push_back(std::shared_ptr<Shape>(node));
}

void Scene::
Add(std::shared_ptr<Shape> node)
{
    nodes.push_back(node);
}

void Scene::
Delete()
{
    nodes.pop_back();
}

void Scene::
Replace(size_t index, std::shared_ptr<Shape> successor)
{
    if (index >= nodes.size()) 
        return;
    nodes[index].swap(successor);
}

void Scene::
Draw(PPMImage & image, int superSampling)
{
    char fileName[]="shapedraw2.cl";
    char kernelName[]="ShapeDraw";

    CLInit(fileName, kernelName);

    CLReadImageBuff(memObj[0], image);

    timespec StartTime, EndTime;
    clock_gettime(CLOCK_REALTIME, &StartTime);

    for (size_t i = 0; i < nodes.size(); i++)
        nodes[i]->Draw(image, superSampling);

    clock_gettime(CLOCK_REALTIME, &EndTime);

    CLWriteImageBuff(memObj[0], image);

    CLRelease();

    int s, ns;
    s=EndTime.tv_sec-StartTime.tv_sec;
    ns=EndTime.tv_nsec-StartTime.tv_nsec;
    if (ns<0)
    {
        ns+=1000000000;
        s--;
    }
    std::cout<<"Total running time "<<s<<" s "<<ns<<" ns."<<std::endl;
}

void Scene::
Draw(unsigned int height, unsigned int width, size_t until, int index, int superSampling)
{
    until = (until < nodes.size()) ? until : nodes.size();
    for (size_t i = 0; i < until; i++)
        nodes[i]->Draw(height, width, index, superSampling);
}

void Scene::
Draw(unsigned int height, unsigned int width, int index, int superSampling)
{
    for (size_t i = 0; i < nodes.size(); i++)
        nodes[i]->Draw(height, width, index, superSampling);
}
