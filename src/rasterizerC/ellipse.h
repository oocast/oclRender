#ifndef __ELLIPSE_H__
#define __ELLIPSE_H__
#include "poly.h"
#include "shape.h"
#include "color.h"
#include "geometry.h"

class Ellipse : public shape
{
	double a, b, c, d, e, f;
	Transform gradient;
	Vector center;
	AABox bound;
public:
	Ellipse(double a, double b, double c,
			double d, double e, double f,
			const Color *color = null_ptr);
	double value(const Vector &p);
	bool contains(const Vector &p);
	Ellipse transform(const Transform &transform);
	void intersections(const Vector &c, const Vector &p, Vector *inter_ps);
	double signed_distance_bound(const Vector &p);
};

Ellipse Circle(const Vector &center, double radius, const Color *color = null_ptr);
#endif //__ELLIPSE_H__