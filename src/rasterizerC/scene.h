#ifndef __SCENE_H__
#define __SCENE_H__
#include <deque>

#include "shape.h"

class Scene : public SceneObject
{
public:
    std::deque<std::shared_ptr<Shape>> nodes;
    // Transform transform;
public:
	Scene(){}
    void Add(Shape *);
    void Add(std::shared_ptr<Shape>);
    virtual void Draw(PPMImage &, int superSampling = 6);
};

#endif //__SCENE_H__
