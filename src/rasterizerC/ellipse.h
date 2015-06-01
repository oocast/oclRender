#ifndef __ELLIPSE_H__
#define __ELLIPSE_H__
#include "poly.h"
#include "shape.h"
#include "color.h"
#include "geometry.h"

class Ellipse : public Shape
{
protected:
  float a, b, c, d, e, f;
  Transform gradient;
  Vector center;
public:
  Ellipse(float a = 1.0, float b = 1.0, float c = 0.0,
    float d = 0.0, float e = 0.0, float f = -1.0,
    const Color *color = nullptr);
	Ellipse(const Ellipse &&ellipse);
  float value(const Vector &p) const;
  bool contains(const Vector &p) const;
  Ellipse transform(const Transform &transform);
  void intersections(const Vector &c, const Vector &p, Vector *inter_ps) const;
  float signed_distance_bound(const Vector &p) const;
	void get_parameters(std::vector<float> &paras, ShapeType *shapeType);
};

Ellipse Circle(const Vector &center, float radius, const Color *color = nullptr);
#endif //__ELLIPSE_H__