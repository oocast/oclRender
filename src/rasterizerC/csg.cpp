#include <cmath>
#include "csg.h"

CSG::
CSG(Shape *v1, Shape *v2, const Color *color) :
Shape(color), v1(v1), v2(v2)
{	
}

/*
Union::
Union(const Shape *v1, const Shape *v2, const Color *color) :
CSG(v1, v2, color)
{
	Vector vs[4] = { v1->bound.low, v1->bound.high,
		v2->bound.low, v2->bound.high };
	bound = AABox::from_vectors(vs, 4);
}

bool Union::
contains(const Vector &p) const
{
	return v1->contains(p) || v2->contains(p);
}
*/

/*
double Union::
signed_distance_bound(const Vector &p) const
{
	double b1, b2;
	b1 = v1->signed_distance_bound(p);
	b2 = v2->signed_distance_bound(p);
	return fmax(b1, b2);
}
*/

/*
Union Union::
transform(const Transform &t)
{
	//return Union(v1->transform(t))
}
*/