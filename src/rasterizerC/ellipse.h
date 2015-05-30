#ifndef __ELLIPSE_H__
#define __ELLIPSE_H__
#include "poly.h"
#include "shape.h"
#include "color.h"
#include "geometry.h"

class Ellipse : public Shape
{
protected:
  double a, b, c, d, e, f;
  Transform gradient;
  Vector center;
public:
  Ellipse(double a = 1.0, double b = 1.0, double c = 0.0,
    double d = 0.0, double e = 0.0, double f = -1.0,
    const Color *color = nullptr);
  double value(const Vector &p) const;
  bool contains(const Vector &p) const;
  Ellipse transform(const Transform &transform);
  void intersections(const Vector &c, const Vector &p, Vector *inter_ps) const;
  double signed_distance_bound(const Vector &p) const;
};

Ellipse Circle(const Vector &center, double radius, const Color *color = nullptr);
#endif //__ELLIPSE_H__