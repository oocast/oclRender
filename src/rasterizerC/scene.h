#ifndef __SCENE_H__
#define __SCENE_H__
#include <deque>
#include <memory>
#include "shape.h"

class Scene : public SceneObject
{
public:
  std::deque<std::shared_ptr<Shape>> Nodes;
  // Transform transform;
public:
  Scene(){}
  void add(Shape *Node);
  virtual void draw(PPMImage &Image, int SuperSampling = 6);
};

#endif //__SCENE_H__