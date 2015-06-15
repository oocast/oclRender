#include "scene.h"
#include "cl_helper.h"

void Scene::
Add(Shape * node)
{
    nodes.push_back(std::shared_ptr<Shape>(node));
}

void Scene::
Draw(PPMImage & image, int superSampling)
{
    CLReadImageBuff(memObj[0], image);

    for (size_t i = 0; i < nodes.size(); i++)
        nodes[i]->Draw(image, superSampling);

    CLWriteImageBuff(memObj[0], image);
}
