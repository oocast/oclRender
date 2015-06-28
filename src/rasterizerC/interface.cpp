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
Ring(float width, float height, float centerX, 
     float centerY, float thickness,
     const Color * inputColorPointer)
{
    float a, b, c, d, e, f;
    a = height * height;
    b = width * width;
    c = 0.0F;
    d = -2 * a * centerX;
    e = -2 * b * centerY;
    f = centerX * centerX * a + centerY * centerY * b - a * b;

    std::shared_ptr<CSG> u(new Union(inputColorPointer, true));
    std::shared_ptr<CSG> in(new Intersection(inputColorPointer, true));
    std::shared_ptr<Shape> outer(new Ellipse(a,b,c,d,e,f,inputColorPointer));

    height -= thickness;
    width -= thickness;
    a = height * height;
    b = width * width;
    c = 0.0F;
    d = -2 * a * centerX;
    e = -2 * b * centerY;
    f = centerX * centerX * a + centerY * centerY * b - a * b;
    std::shared_ptr<Shape> inner(new Ellipse(a, b, c, d, e, f, inputColorPointer, false));
    in->AddElement(outer);
    in->AddElement(inner);
    u->AddElement(in);
    return u;
}