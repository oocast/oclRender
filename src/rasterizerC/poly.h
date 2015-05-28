#ifndef __POLY_H__
#define __POLY_H__
#include <vector>
#include "shape.h"
#include "geometry.h"

class ConvexPoly : public Shape
{
	std::vector<Vector> vs;
	AABox bound;
	std::vector<HalfPlane> half_planes;
public:
	// vertex should be counter-clockwise order
	ConvexPoly(const std::vector<Vector> &ps, const Color *color = null_ptr);
	double signed_distance_bound(const Vector &p);
	bool contains(const Vector &p);
	ConvexPoly transform(const Transform &xform);
};

ConvexPoly Rectangle(const Vector &v1, const Vector &v2, 
					const Color *color = null_ptr);
ConvexPoly LineSegment(const Vector &v1, const Vector &v2, 
					double thickness, const Color *color = null_ptr);


#endif //__POLY_H__