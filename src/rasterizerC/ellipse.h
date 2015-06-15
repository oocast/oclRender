#ifndef __ELLIPSE_H__
#define __ELLIPSE_H__
#include "shape.h"

class Ellipse : public Shape
{
protected:
    float a, b, c, d, e, f;
    Transform gradient;
    Vector center;
public:
    Ellipse(float a = 1.0, float b = 1.0, float c = 0.0,
            float d = 0.0, float e = 0.0, float f = -1.0,
            const Color *colorInputPointer = nullptr, bool positive = true);
    Ellipse(const Ellipse &&);
    float Value(const Vector &) const;
    bool Contains(const Vector &) const;
    Ellipse Transformation(const Transform &);
    std::shared_ptr<Shape> TransformPointer(const Transform &);
    void Intersections(const Vector &, const Vector &, Vector *) const;
    //float signed_distance_bound(const Vector &p) const;
    void GetParameters(std::vector<float> &, ShapeType &);
};

Ellipse Circle(const Vector &, float , const Color *inputColorPointer = nullptr);
#endif //__ELLIPSE_H__
