#include "scene.h"

void Scene::
Add(Shape * node)
{
    nodes.push_back(std::shared_ptr<Shape>(node));
}

void Scene::
Draw(PPMImage & image, int superSampling)
{
    for (size_t i = 0; i < nodes.size(); i++) {
        nodes[i]->Draw(image, superSampling);
    }
}
