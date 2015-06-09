#ifndef __POLY_H__
#define __POLY_H__
#include <vector>
#include <memory>
#include "shape.h"
#include "geometry.h"

class ConvexPoly : public Shape
{
protected:
  std::vector<Vector> Vertices;
  std::vector<HalfPlane> HalfPlanes;
public:
  // vertex should be counter-clockwise order
  //ConvexPoly(){}
  ConvexPoly(const std::vector<Vector> &Points, const Color *InputColorPointer = nullptr);
  ConvexPoly(ConvexPoly &&OtherPolygon);
  //float signed_distance_bound(const Vector &p) const;
  bool contains(const Vector &Point) const;
  ConvexPoly transform(const Transform &Xform);
  std::shared_ptr<Shape> transformPointer(const Transform &Xform);
  void getParameters(std::vector<float> &Parameters, ShapeType *ShapeType);
};

ConvexPoly Triangle(const std::vector<Vector> &Vertices, const Color *InputColorPointer = nullptr);

ConvexPoly Rectangle(const Vector &Vertex1, const Vector &Vertex2, 
        const Color *color = nullptr);

ConvexPoly LineSegment(const Vector &Vertex1, const Vector &Vertex2, 
        float Thickness, const Color *InputColorPointer = nullptr);

#endif //__POLY_H__