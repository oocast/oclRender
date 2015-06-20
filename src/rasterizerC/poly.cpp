#include <cmath>

#include "poly.h"

inline void ConvexPoly::
CalculateExtremum()
{
    bound = AABox::FromVectors(&vertices[0], vertices.size());
    halfPlanes.clear();
    for (size_t i = 0; i < vertices.size(); i++)
        halfPlanes.push_back(HalfPlane(vertices[i], vertices[(i+1) % vertices.size()]));
}

ConvexPoly::
ConvexPoly(const std::vector<Vector> & points, const Color * inputColorPointer,
           bool positive):
Shape(inputColorPointer, positive), vertices(points)
{
    CalculateExtremum();
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
Contains(const Vector & point) const
{
    const HalfPlane * plane;
    for (size_t i = 0; i < halfPlanes.size(); i++) 
    {
        plane = &halfPlanes[i];
        if (plane->SignedDistance(point) < 0)
            return false;
    }
    return true;
}
/*
ConvexPoly ConvexPoly::
Transformation(const Transform & xform)
{
    std::vector<Vector> xformedVertices;
    for (size_t i = 0; i < vertices.size(); i++)
        xformedVertices.push_back(xform * vertices[i]);
    return ConvexPoly(xformedVertices, &shapeColor);
}
*/
ConvexPoly & ConvexPoly::
Transformation(const Transform & xform)
{
    //std::vector<Vector> xformedVertices;
    for (size_t i = 0; i < vertices.size(); i++)
        //xformedVertices.push_back(xform * vertices[i]);
        vertices[i]=xform*vertices[i];
    CalculateExtremum();
    return *this;
}

std::shared_ptr<Shape> ConvexPoly::
TransformPointer(const Transform & xform)
{
    ConvexPoly *temp = new ConvexPoly(*this);
    temp->Transformation(xform);
    std::shared_ptr<Shape> result(temp);
    return (std::shared_ptr<Shape>) result;
}

void ConvexPoly::
GetParameters(std::vector<float> & parameters, std::vector<int> & structures)
{
    for (auto it = halfPlanes.begin(); it != halfPlanes.end(); it++) 
    {
        parameters.push_back(it->ab.x); // a
        parameters.push_back(it->ab.y); // b
        parameters.push_back(it->c);
    }
    structures.push_back(positive);
    structures.push_back(vertices.size());
}



ConvexPoly 
Triangle(const std::vector<Vector> &vertices, const Color * inputColorPointer)
{
    return ConvexPoly(vertices, inputColorPointer);
}

ConvexPoly
Rectangle(const Vector &vertex1, const Vector &vertex2, 
const Color * inputColorPointer)
{
    std::vector<Vector> vertices;
    vertices.push_back(Vector(fmin(vertex1.x, vertex2.x), fmin(vertex1.y, vertex2.y)));
    vertices.push_back(Vector(fmax(vertex1.x, vertex2.x), fmin(vertex1.y, vertex2.y)));
    vertices.push_back(Vector(fmax(vertex1.x, vertex2.x), fmax(vertex1.y, vertex2.y)));
    vertices.push_back(Vector(fmin(vertex1.x, vertex2.x), fmax(vertex1.y, vertex2.y)));
    return ConvexPoly(vertices, inputColorPointer);
}

ConvexPoly
LineSegment(const Vector &vertex1, const Vector &vertex2, 
            float thickness, const Color *inputColorPointer)
{
    Vector d = vertex2 - vertex1;
    float tmp = d.x;
    d.x = -d.y;
    d.y = tmp;
    d = d * (thickness / d.Length() / 2);
    std::vector<Vector> vertices;
    vertices.push_back(vertex1 + d);
    vertices.push_back(vertex1 - d);
    vertices.push_back(vertex2 - d);
    vertices.push_back(vertex2 + d);
    return ConvexPoly(vertices, inputColorPointer);
}
