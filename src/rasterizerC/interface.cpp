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

std::shared_ptr<Shape> 
Arrow(const Vector &boundVertex1, const Vector &boundVertex2,
      int direction, const Color * inputColorPointer,
      float lengthRatio, float widthRatio)
{
    Vector low = boundVertex1.Min(boundVertex2);
    Vector high = boundVertex1.Max(boundVertex2);
    Vector size = high - low;
    Vector handleVertex1, handleVertex2, triangleVertex1, triangleVertex2, triangleVertex3;
    float xin = 0.5F * (1.0F - widthRatio) * size.x;
    float yin = 0.5F * (1.0F - widthRatio) * size.y;

    // direction: 0right, 1up, 2left, 3down
    switch (direction)
    {
    case 0:
        handleVertex1 = low + Vector(0.0F, yin);
        handleVertex2 = Vector(low.x + lengthRatio * size.x, high.y - yin);
        triangleVertex1 = high - Vector(0.0F, 0.5F * size.y);
        triangleVertex2 = Vector(handleVertex2.x, high.y);
        triangleVertex3 = Vector(handleVertex2.x, low.y);
        break;
    case 1:
        handleVertex1 = low + Vector(xin);
        handleVertex2 = Vector(high.x - xin, low.y + lengthRatio * size.y);
        triangleVertex1 = high - Vector(0.5 * size.x);
        triangleVertex2 = Vector(low.x, handleVertex2.y);
        triangleVertex3 = Vector(high.x, handleVertex2.y);
        break;
    case 2:
        handleVertex1 = Vector(high.x - lengthRatio * size.x, low.y + yin);
        handleVertex2 = high - Vector(0.0F, yin);
        triangleVertex1 = low + Vector(0.0F, 0.5 * size.y);
        triangleVertex2 = Vector(handleVertex1.x, low.y);
        triangleVertex3 = Vector(handleVertex1.x, high.y);
        break;
    case 3:
        handleVertex1 = Vector(low.x + xin, high.y - lengthRatio * size.y);
        handleVertex2 = high - Vector(xin);
        triangleVertex1 = low + Vector(0.5 * size.x);
        triangleVertex2 = Vector(high.x, handleVertex1.y);
        triangleVertex3 = Vector(low.x, handleVertex1.y);
        break;
    }

    std::shared_ptr<CSG> u(new Union(inputColorPointer, true));
    std::shared_ptr<CSG> in1(new Intersection(inputColorPointer, true));
    std::shared_ptr<CSG> in2(new Intersection(inputColorPointer, true));
    std::shared_ptr<Shape> rect(new ConvexPoly(Rectangle(handleVertex1, handleVertex2)));
    std::shared_ptr<Shape> tri(new ConvexPoly(Triangle({triangleVertex1, triangleVertex2, triangleVertex3})));
    in1->AddElement(rect);
    in2->AddElement(tri);
    u->AddElement(in1);
    u->AddElement(in2);
    return u;
}