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
    // UI compatible constructor
    Ellipse(const Vector &, const Vector &,
            const Color *colorInputPointer = nullptr, bool positive = true);
    Ellipse(const Ellipse &&);
    Ellipse(const Ellipse &);
    float Value(const Vector &) const;
    bool Contains(const Vector &) const;
    //Ellipse Transformation(const Transform &);
    Ellipse & Transformation(const Transform &);
    std::shared_ptr<Shape> TransformPointer(const Transform &);
    void Intersections(const Vector &, const Vector &, Vector *) const;
    //float signed_distance_bound(const Vector &p) const;
    void GetParameters(std::vector<float> &, std::vector<int> &);
    void CalculateBound(float){}
    std::shared_ptr<Shape> AccessElement(size_t){ return nullptr; }
    Vector AccessCenter() {return center;}
private:
    void CalculateExtremum();
};

Ellipse Circle(const Vector &, float , const Color *inputColorPointer = nullptr, bool positive = true);
#endif //__ELLIPSE_H__
