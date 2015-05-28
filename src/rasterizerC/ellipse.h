#ifndef __ELLIPSE_H__
#define __ELLIPSE_H__
#include "poly.h"
#include "shape.h"
#include "color.h"
#include "geometry.h"

class Ellipse : public Shape
{
	double a, b, c, d, e, f;
	Transform gradient;
	Vector center;
	AABox bound;
public:
	Ellipse(double a = 1.0, double b = 1.0, double c = 0.0,
		double d = 0.0, double e = 0.0, double f = -1.0,
		const Color *color = null_ptr);
	double value(const Vector &p);
	bool contains(const Vector &p);
	Ellipse transform(const Transform &transform);
	void intersections(const Vector &c, const Vector &p, Vector *inter_ps);
	double signed_distance_bound(const Vector &p);
};

Ellipse Circle(const Vector &center, double radius, const Color *color = null_ptr);
#endif //__ELLIPSE_H__