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
    bool positive;
public:
    Shape(const Color *colorInputPointer = nullptr, bool positive = true);
    virtual bool Contains(const Vector &) const = 0;
    //virtual float signed_distance_bound(const Vector &p) const = 0;
    void Draw(PPMImage &, int);
    // draw shape on camera frame
    void Draw(unsigned int, unsigned int, int, int);
    virtual void GetParameters(std::vector<float> &, std::vector<int> &) = 0;
    virtual std::shared_ptr<Shape> TransformPointer(const Transform &) = 0;
    // recompute bound of shape, used only by CSG
    virtual void CalculateBound(float) = 0;
    // TODO: transform return true caller (derived) class obj
    //template <class Shape_drv> // derived class of Shape
    //virtual Shape_drv transform(const Transform &transform) const = 0;
    virtual std::shared_ptr<Shape> AccessElement(size_t) = 0;
private:
    void DrawRGB(PPMImage &, int);
    void DrawYUYV(PPMImage &, int);
};

#endif //__SHAPE_H__
