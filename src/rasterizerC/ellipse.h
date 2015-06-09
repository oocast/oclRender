#ifndef __ELLIPSE_H__
#define __ELLIPSE_H__
#include <memory>
#include "poly.h"
#include "shape.h"
#include "color.h"
#include "geometry.h"

class Ellipse : public Shape
{
protected:
  float a, b, c, d, e, f;
  Transform Gradient;
  Vector Center;
public:
  Ellipse(float a = 1.0, float b = 1.0, float c = 0.0,
    float d = 0.0, float e = 0.0, float f = -1.0,
		const Color *ColorInputPointer = nullptr);
	Ellipse(const Ellipse &&OtherEllipse);
  float value(const Vector &Point) const;
  bool contains(const Vector &Point) const;
  Ellipse transform(const Transform &Xform);
	std::shared_ptr<Shape> transformPointer(const Transform &Xform);
  void intersections(const Vector &Point1, const Vector &Point2, Vector *IntersectionPoints) const;
  //float signed_distance_bound(const Vector &p) const;
	void getParameters(std::vector<float> &Parameters, ShapeType *ShapeType);
};

Ellipse Circle(const Vector &Center, float Radius, const Color *InputColorPointer = nullptr);
#endif //__ELLIPSE_H__