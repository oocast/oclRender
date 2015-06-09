#include "poly.h"
#include <cmath>

ConvexPoly::
ConvexPoly(const std::vector<Vector> &Points, const Color *InputColorPointer):
Shape(InputColorPointer), Vertices(Points)
{
  Bound = AABox::fromVectors(&Vertices[0], Vertices.size());
  for (size_t i = 0; i < Vertices.size(); i++) {
    HalfPlanes.push_back(HalfPlane(Vertices[i], Vertices[(i+1) % Vertices.size()]));
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

/*
float ConvexPoly::
signed_distance_bound(const Vector &p) const
{
  const HalfPlane *plane = &half_planes[0];
  float min_inside, max_outside;
  min_inside = 1e15F;
  max_outside = -1e15F;
  for (size_t i = 0; i < half_planes.size(); i++) {
    plane = &half_planes[i];
    float d = plane->signed_distance(p);
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
*/

bool ConvexPoly::
contains(const Vector &Point) const
{
  const HalfPlane *plane;
  for (size_t i = 0; i < HalfPlanes.size(); i++) {
    plane = &HalfPlanes[i];
    if (plane->signedDistance(Point) < 0) {
      return false;
    }
  }
  return true;
}

ConvexPoly ConvexPoly::
transform(const Transform &Xform)
{
  std::vector<Vector> xformedVertices;
  for (size_t i = 0; i < Vertices.size(); i++) {
    xformedVertices.push_back(Xform * Vertices[i]);
  }
  return ConvexPoly(xformedVertices, &ShapeColor);
}

std::shared_ptr<Shape> ConvexPoly::
transformPointer(const Transform &Xform)
{
  std::shared_ptr<Shape> result(new ConvexPoly(transform(Xform)));
  return result;
}

void ConvexPoly::
getParameters(std::vector<float> &Parameters, ShapeType *ShapeType)
{
  *ShapeType = CONVEXPOLY;
  for (auto it = HalfPlanes.begin(); it != HalfPlanes.end(); it++) {
    Parameters.push_back(it->ab.x); // a
    Parameters.push_back(it->ab.y); // b
    Parameters.push_back(it->c);
  }
}

ConvexPoly 
Triangle(const std::vector<Vector> &Vertices, const Color *InputColorPointer)
{
  return ConvexPoly(Vertices, InputColorPointer);
}

ConvexPoly
Rectangle(const Vector &Vertex1, const Vector &Vertex2, 
const Color *InputColorPointer)
{
  std::vector<Vector> Vertices;
  Vertices.push_back(Vector(fmin(Vertex1.x, Vertex2.x), fmin(Vertex1.y, Vertex2.y)));
  Vertices.push_back(Vector(fmax(Vertex1.x, Vertex2.x), fmin(Vertex1.y, Vertex2.y)));
  Vertices.push_back(Vector(fmax(Vertex1.x, Vertex2.x), fmax(Vertex1.y, Vertex2.y)));
  Vertices.push_back(Vector(fmin(Vertex1.x, Vertex2.x), fmax(Vertex1.y, Vertex2.y)));
  return ConvexPoly(Vertices, InputColorPointer);
}

ConvexPoly
LineSegment(const Vector &Vertex1, const Vector &Vertex2, 
      float Thickness, const Color *InputColorPointer)
{
  Vector d = Vertex2 - Vertex1;
  float tmp = d.x;
  d.x = -d.y;
  d.y = tmp;
  d = d * (Thickness / d.length() / 2);
  std::vector<Vector> Vertices;
  Vertices.push_back(Vertex1 + d);
  Vertices.push_back(Vertex1 - d);
  Vertices.push_back(Vertex2 - d);
  Vertices.push_back(Vertex2 + d);
  return ConvexPoly(Vertices, InputColorPointer);
}