#ifndef __SHAPE_H__
#define __SHAPE_H__
#include <memory>

#include "image.h"

enum ShapeType
{
    ELLIPSE, CONVEXPOLY
};

class SceneObject
{
public:
    virtual void Draw(PPMImage &, int superSampling) = 0;
};

class Shape : public SceneObject
{
public:
    Color shapeColor;
    AABox bound;
public:
    Shape(const Color *colorInputPointer = nullptr);
    virtual bool Contains(const Vector &) const = 0;
    //virtual float signed_distance_bound(const Vector &p) const = 0;
    void Draw(PPMImage &, int);
    virtual void GetParameters(std::vector<float> &, ShapeType &) = 0;
    virtual std::shared_ptr<Shape> TransformPointer(const Transform &) = 0;
    // TODO: transform return true caller (derived) class obj
    //template <class Shape_drv> // derived class of Shape
    //virtual Shape_drv transform(const Transform &transform) const = 0;
};

#endif //__SHAPE_H__
