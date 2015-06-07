#ifndef __SCENE_H__
#define __SCENE_H__
#include <deque>
#include <memory>
#include "shape.h"

class Scene : public SceneObject
{
public:
  std::deque<std::shared_ptr<Shape>> nodes;
  // Transform transform;
public:
	Scene(){}
  void add(Shape *node);
  virtual void draw(PPMImage &image, int super_sampling = 6);
};

#endif //__SCENE_H__