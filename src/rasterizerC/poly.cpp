#include "poly.h"
#include <cmath>

ConvexPoly::
ConvexPoly(const std::vector<Vector> &ps, const Color *color):
Shape(color), vs(ps)
{
  bound = AABox::from_vectors(&vs[0], vs.size());
  for (size_t i = 0; i < vs.size(); i++) {
    half_planes.push_back(HalfPlane(vs[i], vs[(i+1) % vs.size()]));
  }
}

/*
ConvexPoly::
ConvexPoly(ConvexPoly &&poly):
Shape(&poly.color), vs(poly.vs), half_planes(poly.half_planes)
{
	bound = poly.bound;
	poly.vs.clear();
	poly.half_planes.clear();
}
*/

double ConvexPoly::
signed_distance_bound(const Vector &p) const
{
  const HalfPlane *plane = &half_planes[0];
  double min_inside, max_outside;
  min_inside = 1e30;
  max_outside = -1e30;
  for (size_t i = 0; i < half_planes.size(); i++) {
    plane = &half_planes[i];
    double d = plane->signed_distance(p);
    if (d <= 0 && d > max_outside) {
      max_outside = d;
    }
    if (d >= 0 && d < min_inside) {
      min_inside = d;
    }
  }
  if (max_outside != -1e30) {
    return max_outside;
  }
  else {
    return min_inside;
  }
}

bool ConvexPoly::
contains(const Vector &p) const
{
  const HalfPlane *plane;
  for (size_t i = 0; i < half_planes.size(); i++) {
    plane = &half_planes[i];
    if (plane->signed_distance(p) < 0) {
      return false;
    }
  }
  return true;
}

ConvexPoly ConvexPoly::
transform(const Transform &xform)
{
  std::vector<Vector> xvs;
  for (size_t i = 0; i < vs.size(); i++) {
    xvs.push_back(xform * vs[i]);
  }
  return ConvexPoly(xvs, &color);
}

ConvexPoly 
Triangle(const std::vector<Vector> &ps, const Color *color)
{
	return ConvexPoly(ps, color);
}

ConvexPoly
Rectangle(const Vector &v1, const Vector &v2, 
const Color *color)
{
  std::vector<Vector> vs;
  vs.push_back(Vector(fmin(v1.x, v2.x), fmin(v1.y, v2.y)));
  vs.push_back(Vector(fmax(v1.x, v2.x), fmin(v1.y, v2.y)));
  vs.push_back(Vector(fmax(v1.x, v2.x), fmax(v1.y, v2.y)));
  vs.push_back(Vector(fmin(v1.x, v2.x), fmax(v1.y, v2.y)));
  return ConvexPoly(vs, color);
}

ConvexPoly
LineSegment(const Vector &v1, const Vector &v2, 
      double thickness, const Color *color)
{
  Vector d = v2 - v1;
  double tmp = d.x;
  d.x = -d.y;
  d.y = tmp;
  d = d * (thickness / d.length() / 2);
  std::vector<Vector> vs;
  vs.push_back(v1 + d);
  vs.push_back(v1 - d);
  vs.push_back(v2 - d);
  vs.push_back(v2 + d);
  return ConvexPoly(vs, color);
}