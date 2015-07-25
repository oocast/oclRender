#include <cmath>

#include "ellipse.h"

inline void Ellipse::
CalculateExtremum()
{
    if (c*c - 4*a*b < 0)
    {
        //throw std::invalid_argument("Not an ellipse");

        gradient = Transform(2*a, c, d, c, 2*b, e);
        center = gradient.Inverse() * Vector(0.0, 0.0);
        float x[2], y[2];
        Quadratic(b-c*c/(4*a), e-c*d/(2*a), f-d*d/(4*a), y);
        Quadratic(a-c*c/(4*b), d-c*e/(2*b), f-e*e/(4*b), x);
        // y bounds: dv/dx(x, y) = 0, v(x,y) = 0
        Vector boundVectors[4] = {
            Vector(-(d + c*y[0])/(2*a), y[0]),
            Vector(-(d + c*y[1])/(2*a), y[1]),
            Vector(x[0], -(e + c*x[0])/(2*b)),
            Vector(x[1], -(e + c*x[1])/(2*b))
        };
        bound = AABox::FromVectors(boundVectors, 4);
        if (!Contains(center));
            //throw std::logic_error("Internal error, center not inside ellipse");
    }
    else //if (c*c==4*a*b)
        bound=AABox(Vector(), Vector(1, 0.75));
}

Ellipse::
Ellipse(float a, float b, float c,
        float d, float e, float f,
        const Color *colorInputPointer,
        bool positive) :
Shape(colorInputPointer, positive), a(a), b(b), c(c),
d(d), e(e), f(f)
{
    CalculateExtremum();
}

Ellipse::
Ellipse(const Vector &boundVertex1, const Vector &boundVertex2,
        const Color *colorInputPointer, bool positive) :
Shape(colorInputPointer, positive)
{
    Vector center = (boundVertex1 + boundVertex2) * 0.5F;
    Vector halfSize = boundVertex1.Max(boundVertex2) - center;
    a = halfSize.y * halfSize.y;
    b = halfSize.x * halfSize.x;
    c = 0.0F;
    d = -2.0 * center.x * a;
    e = -2.0 * center.y * b;
    f = a * center.x * center.x + b * center.y * center.y - a * b;
    CalculateExtremum();
}

Ellipse::
Ellipse(const Ellipse &ellipse) :
Shape(&ellipse.shapeColor, ellipse.positive),
a(ellipse.a), b(ellipse.b), c(ellipse.c),
d(ellipse.d), e(ellipse.e), f(ellipse.f)
{
    CalculateExtremum();
}

float Ellipse::
Value(const Vector & point) const
{
    return (a*point.x*point.x + b*point.y*point.y + c*point.x*point.y +
            d*point.x + e*point.y + f);
}

bool Ellipse::
Contains(const Vector &point) const
{
    return ((Value(point) < 0)==positive);
}
/*
Ellipse Ellipse::
Transformation(const Transform & xform)
{
    Transform i(xform.Inverse());
    float aa, bb, cc, dd, ee, ff;
    float m00, m01, m02, m10, m11, m12;
    m00 = i.m[0][0]; m01 = i.m[0][1]; m02 = i.m[0][2];
    m10 = i.m[1][0]; m11 = i.m[1][1]; m12 = i.m[1][2];
    aa = a*m00*m00 + b*m10*m10 + c*m00*m10;
    bb = a*m01*m01 + b*m11*m11 + c*m01*m11;
    cc = 2*a*m00*m01 + 2*b*m10*m11 + c*(m00*m11 + m01*m10);
    dd = 2*a*m00*m02 + 2*b*m10*m12 +
         c*(m00*m12 + m02*m10) + d*m00 + e*m10;
    ee = 2*a*m01*m02 + 2*b*m11*m12 +
         c*(m01*m12 + m02*m11) + d*m01 + e*m11;
    ff = a*m02*m02 + b*m12*m12 + c*m02*m12 +
         d*m02 + e*m12 + f;
    return Ellipse(aa, bb, cc, dd, ee, ff, &shapeColor);
}
*/

Ellipse & Ellipse::
Transformation(const Transform & xform)
{
    Transform i(xform.Inverse());
    float aa, bb, cc, dd, ee, ff;
    float m00, m01, m02, m10, m11, m12;
    m00 = i.m[0][0]; m01 = i.m[0][1]; m02 = i.m[0][2];
    m10 = i.m[1][0]; m11 = i.m[1][1]; m12 = i.m[1][2];
    aa = a*m00*m00 + b*m10*m10 + c*m00*m10;
    bb = a*m01*m01 + b*m11*m11 + c*m01*m11;
    cc = 2*a*m00*m01 + 2*b*m10*m11 + c*(m00*m11 + m01*m10);
    dd = 2*a*m00*m02 + 2*b*m10*m12 +
         c*(m00*m12 + m02*m10) + d*m00 + e*m10;
    ee = 2*a*m01*m02 + 2*b*m11*m12 +
         c*(m01*m12 + m02*m11) + d*m01 + e*m11;
    ff = a*m02*m02 + b*m12*m12 + c*m02*m12 +
         d*m02 + e*m12 + f;
    a=aa;
    b=bb;
    c=cc;
    d=dd;
    e=ee;
    f=ff;
    CalculateExtremum();
    return *this;
}

std::shared_ptr<Shape> Ellipse::
TransformPointer(const Transform &xform)
{
    Ellipse * temp = new Ellipse(*this);
    temp->Transformation(xform);
    //std::shared_ptr<Shape> result(temp);
    return (std::shared_ptr<Shape>) temp;
}

void Ellipse::
Intersections(const Vector & point1, const Vector & point2, Vector * intersectionPoints) const
{
    // returns the two intersections of the line through Point1 and Point2
        // and the ellipse. Defining a line as a function of a single
        // parameter u, x(u) = Point1.x + u * (Point2.x - Point1.x), (and same for y)
        // this simply solves the quadratic equation f(x(u), y(u)) = 0
    Vector pc = point2 - point1;
    float u2, u1, u0;
    u2 = a*pc.x*pc.x + b*pc.y*pc.y + c*pc.x*pc.y;
    u1 = 2*a*point1.x*pc.x + 2*b*point1.y*pc.y + 
         c*point1.y*pc.x + c*point1.x*pc.y + d*pc.x + 
         e*pc.y;
    u0 = a*point1.x*point1.x + b*point1.y*point1.y + c*point1.x*point1.y + 
         d*point1.x + e*point1.y + f;
    /*
    // try catch block for imaginary solutions
    try{
    }
    catch{
    }
    */
    float sols[2];
    Quadratic(u2, u1, u0, sols);
    intersectionPoints[0] = point1+pc*sols[0];
    intersectionPoints[1] = point1+pc*sols[1];
}

/*
float Ellipse::
signed_distance_bound(const Vector &p) const
{
    float v = value(p);
    if (v == 0.0)
        return 0.0;
    else if (v < 0.0) {
                // if inside the ellipse, create an inscribed quadrilateral
                        // that contains the given point and use the minimum distance
                        // from the point to the quadrilateral as a bound. Since
                        // the quadrilateral lies entirely inside the ellipse, the
                        // distance from the point to the ellipse must be larger.
            Vector v02[2], v13[2];
            intersections(p, p + Vector(1.0, 0.0), v02);
            intersections(p, p + Vector(0.0, 1.0), v13);
            std::vector<Vector> polyVertices;
            polyVertices.push_back(v02[0]);
            polyVertices.push_back(v13[0]);
            polyVertices.push_back(v02[1]);
            polyVertices.push_back(v13[1]);
            return std::abs(ConvexPoly(polyVertices).signed_distance_bound(p));
    }
    else {
        Vector c = center;
        Vector crossings[2];
        size_t surface_index;
        intersections(c, p, crossings);
        if ((p - crossings[0]).length() < (p - crossings[1]).length()) { // TODO: optimize condition
            surface_index = 0;
        }
        else {
            surface_index = 1;
        }
        // n is normal at surface_pt
        Vector n = gradient * crossings[surface_index];
        n = n * (1.0F / n.length());
        // returns the length of the projection of p - surface_pt
        // along the normal
        return -std::abs(n.dot(p - crossings[surface_index]));
    }
}
*/

void Ellipse::
GetParameters(std::vector<float> &parameters, std::vector<int> & structures)
{
    parameters.push_back(a);
    parameters.push_back(b);
    parameters.push_back(c);
    parameters.push_back(0);
    parameters.push_back(d);
    parameters.push_back(e);
    parameters.push_back(f);
    parameters.push_back(0);
    structures.push_back(positive);
    structures.push_back(0);
}

Ellipse 
Circle(const Vector &center, float radius, const Color * inputColorPointer, bool positive)
{
    Ellipse circle(1.0, 1.0, 0.0, 0.0, 0.0, -1.0, inputColorPointer, positive);
    circle.Transformation(Scale(radius, radius));
    circle.Transformation(Translate(center.x, center.y));
    return circle;
}
