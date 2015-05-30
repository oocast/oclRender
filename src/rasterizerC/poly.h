#ifndef __POLY_H__
#define __POLY_H__
#include <vector>
#include "shape.h"
#include "geometry.h"

class ConvexPoly : public Shape
{
protected:
  std::vector<Vector> vs;
  std::vector<HalfPlane> half_planes;
public:
  // vertex should be counter-clockwise order
	//ConvexPoly(){}
  ConvexPoly(const std::vector<Vector> &ps, const Color *color = nullptr);
	ConvexPoly(const ConvexPoly &&poly);
  double signed_distance_bound(const Vector &p) const;
  bool contains(const Vector &p) const;
  ConvexPoly transform(const Transform &xform);
};

ConvexPoly Triangle(const std::vector<Vector> &ps, const Color *color = nullptr);

ConvexPoly Rectangle(const Vector &v1, const Vector &v2, 
				const Color *color = nullptr);

ConvexPoly LineSegment(const Vector &v1, const Vector &v2, 
				double thickness, const Color *color = nullptr);

#endif //__POLY_H__