#include "scene.h"

void Scene::
add(Shape *node)
{
  nodes.push_back(node);
}

void Scene::
draw(PPMImage &image, int super_sampling)
{
  for (size_t i = 0; i < nodes.size(); i++) {
    nodes[i]->draw(image, super_sampling);
  }
}