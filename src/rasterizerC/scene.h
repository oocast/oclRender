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
    // Delete last element
    void Delete();
    // Draw on picture, execute once
    virtual void Draw(PPMImage &, int superSampling = 6);
    // Draw on camera frame, execute repeatedly
    void Draw(unsigned int, unsigned int, int, int superSampling = 6);
    // Draw untill certain shape in scene, used by demo
    void Draw(unsigned int, unsigned int, size_t, int, int superSampling = 6);
    
};

#endif //__SCENE_H__
