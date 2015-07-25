#ifndef __POLY_H__
#define __POLY_H__

#include "shape.h"

class ConvexPoly : public Shape
{
protected:
    std::vector<Vector> vertices;
    std::vector<HalfPlane> halfPlanes;
public:
    // vertex should be counter-clockwise order
    //ConvexPoly(){}
    ConvexPoly(const std::vector<Vector> &, 
							 const Color * inputColorPointer = nullptr,
							 bool positive = true);
    ConvexPoly(ConvexPoly &&);
    ConvexPoly(const ConvexPoly &);
    //float signed_distance_bound(const Vector &p) const;
    bool Contains(const Vector &) const;
    //ConvexPoly Transformation(const Transform &);
    ConvexPoly & Transformation(const Transform &);
    std::shared_ptr<Shape> TransformPointer(const Transform &);
    void GetParameters(std::vector<float> &, std::vector<int> &);
    void CalculateBound(float){}
    std::shared_ptr<Shape> AccessElement(size_t){ return nullptr; }
private:
    void CalculateExtremum();
};

ConvexPoly Triangle(const std::vector<Vector> &, const Color * inputColorPointer = nullptr, bool positive = true);

ConvexPoly Rectangle(const Vector &, const Vector &, const Color * color = nullptr, bool positive = true);

ConvexPoly LineSegment(const Vector &, const Vector &, float, const Color * inputColorPointer = nullptr, bool positive = true);

#endif //__POLY_H__
