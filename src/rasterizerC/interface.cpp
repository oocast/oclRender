#include "interface.h"

std::shared_ptr<Shape> 
Frame(const Vector &vertex1, const Vector &vertex2, float thickness,
      const Color * inputColorPointer)
{
    Vector low = vertex1.Min(vertex2);
    Vector high = vertex2.Max(vertex2);
    Vector innerWay(thickness, thickness);
    low = low + innerWay;
    high = high - innerWay;

    std::shared_ptr<CSG> u(new Union(inputColorPointer, true));
    std::shared_ptr<CSG> in(new Intersection(inputColorPointer, true));
    std::shared_ptr<Shape> outer(new ConvexPoly(Rectangle(vertex1, vertex2, inputColorPointer)));
    std::shared_ptr<Shape> inner(new ConvexPoly(Rectangle(low, high, inputColorPointer, false)));
    in->AddElement(outer);
    in->AddElement(inner);
    u->AddElement(in);
    return u;
}

std::shared_ptr<Shape> 
Ring(float halfWidth, float halfHeight, float centerX, 
     float centerY, float thickness,
     const Color * inputColorPointer)
{
    Vector center(centerX, centerY);
    Vector halfSize(halfWidth, halfHeight);
    Vector low = center - halfSize;
    Vector high = center + halfSize;

    std::shared_ptr<CSG> u(new Union(inputColorPointer, true));
    std::shared_ptr<CSG> in(new Intersection(inputColorPointer, true));
    std::shared_ptr<Shape> outer(new Ellipse(low, high, inputColorPointer));

    Vector innerWay(thickness, thickness);
    low = low + innerWay;
    high = high - innerWay;
    std::shared_ptr<Shape> inner(new Ellipse(low, high, inputColorPointer, false));
    in->AddElement(outer);
    in->AddElement(inner);
    u->AddElement(in);
    return u;
}

std::shared_ptr<Shape> 
Ring(const Vector &boundVertex1, const Vector &boundVertex2, float thickness,
     const Color * inputColorPointer)
{
    Vector center = (boundVertex1 + boundVertex2) * 0.5F;
    Vector halfSize = boundVertex1.Max(boundVertex2) - center;
    Vector low = center - halfSize;
    Vector high = center + halfSize;

    std::shared_ptr<CSG> u(new Union(inputColorPointer, true));
    std::shared_ptr<CSG> in(new Intersection(inputColorPointer, true));
    std::shared_ptr<Shape> outer(new Ellipse(low, high, inputColorPointer));

    Vector innerWay(thickness, thickness);
    low = low + innerWay;
    high = high - innerWay;
    std::shared_ptr<Shape> inner(new Ellipse(low, high, inputColorPointer, false));
    in->AddElement(outer);
    in->AddElement(inner);
    u->AddElement(in);
    return u;
}