#include "scene.h"

void Scene::
add(Shape *Node)
{
  Nodes.push_back(std::shared_ptr<Shape>(Node));
}

void Scene::
draw(PPMImage &Image, int SuperSampling)
{
  for (size_t i = 0; i < Nodes.size(); i++) {
    Nodes[i]->draw(Image, SuperSampling);
  }
}