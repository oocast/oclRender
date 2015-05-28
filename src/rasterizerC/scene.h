#ifndef __SCENE_H__
#define __SCENE_H__
#include <deque>
#include "shape.h"

class Scene : public SceneObject
{
public:
	std::deque<Shape> nodes;
	// Transform transform;
public:
	Scene();
	void add(const Shape &node);
	virtual void draw(PPMImage &image, int super_sampling = 6);
};

#endif //__SCENE_H__