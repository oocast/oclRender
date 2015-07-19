#include <cmath>

#include "geometry.h"

void 
Quadratic(float a, float b, float c, float * x)
{
    if (a == 0.0) 
    {
        x[0] = -c/b;
        x[1] = -c/b;
    }
    float d = sqrt(b * b - 4 * a * c);    
    if (b >= 0)
    {
        x[0] = (-b - d) / (2 * a);
        x[1] = (2 * c) / (-b - d);
    }
    else 
    {
        x[0] = (2 * c) / (-b + d);
        x[1] = (-b + d) / (2 * a);
    }
}

Vector::
Vector(float xIn, float yIn):
x(xIn), y(yIn)
{}

Vector Vector::
operator + (const Vector & o) const
{
    return Vector(x + o.x, y + o.y);
}

Vector Vector::
operator - (const Vector & o) const
{
    return Vector(x - o.x, y - o.y);
}

Vector Vector::
operator * (float k) const
{
    return Vector(x * k, y * k);
}

float Vector::
Dot(const Vector & o) const
{
    return x * o.x + y * o.y;
}

Vector Vector::
Min(const Vector & o) const
{
    return Vector(fmin(x, o.x), fmin(y, o.y));
}

Vector Vector::
Max(const Vector & o) const
{
    return Vector(fmax(x, o.x), fmax(y, o.y));
}

float Vector::
Length()
{
    return sqrt(x * x + y * y);
}

AABox::
AABox(const Vector & p1, const Vector & p2):
low(p1.Min(p2)), high(p2.Max(p2))
{}

Vector AABox::
Midpoint() const
{
    return (low + high) * 0.5;
}

Vector AABox::
Size() const
{
    return high - low;
}

bool AABox::
Contains(const Vector & point) const
{
    return (low.x <= point.x)  &&
           (point.x <= high.x) &&
           (low.y <= point.y)  &&
           (point.y <= high.y);
}

bool AABox::
Overlaps(const AABox & other) const
{
    return !((other.low.x >= high.x) ||
             (other.high.x <= low.x) ||
             (other.low.y >= high.y) ||
             (other.high.y <= low.y));
}

AABox AABox::
Intersection(const AABox & other) const
{
    return AABox(low.Max(other.low), high.Min(other.high));
}

AABox AABox::
FromVectors(const Vector * vectors, const int vectorNum)
{
    Vector tmpLow = vectors[0], tmpHigh = vectors[0];
    for (int i = 1; i < vectorNum; i++) 
    {
        tmpLow = tmpLow.Min(vectors[i]);
        tmpHigh = tmpHigh.Max(vectors[i]);
    }
    return AABox(tmpLow, tmpHigh);
}

HalfPlane::
HalfPlane(const Vector & point1, const Vector & point2):
ab(-point2.y + point1.y, point2.x - point1.x)
{
    float l = ab.Length();
    c = -ab.Dot(point1) / l;
    ab = ab * (1.0F/ l);
}

float HalfPlane::
SignedDistance(const Vector & point) const
{
    return ab.Dot(point) + c;
}

Transform::
Transform(float m11, float m12, float tx,
            float m21, float m22, float ty) 
{
    m[0][0] = m11; m[0][1] = m12; m[0][2] = tx;
    m[1][0] = m21; m[1][1] = m22; m[1][2] = ty;
    m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0;
}

const Transform Transform::
operator * (const Transform & other) const
{
    float t[2][3] = {{0.0, 0.0, 0.0},
                     {0.0, 0.0, 0.0}};    
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                t[i][j] += m[i][k] * other.m[k][j];

    return Transform(t[0][0], t[0][1], t[0][2],
                     t[1][0], t[1][1], t[1][2]);
}

const Vector Transform::
operator * (const Vector & other) const
{
    float nx = m[0][0] * other.x + m[0][1] * other.y + m[0][2];
    float ny = m[1][0] * other.x + m[1][1] * other.y + m[1][2];
    return Vector(nx, ny);
}

float Transform::
Det() const
{
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

Transform Transform::
Inverse() const
{
    float d = 1.0F / Det();
    Transform t = Transform(d * m[1][1], -d * m[0][1], 0, 
                            -d * m[1][0], d * m[0][0], 0);
    Vector v = t * Vector(m[0][2], m[1][2]);
    t.m[0][2] = -v.x;
    t.m[1][2] = -v.y;
    return t;
}

const Transform
Identity()
{
    return Transform(1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

const Transform
Rotate(float theta)
{
    float s = sin(theta);
    float c = cos(theta);
    return Transform(c, -s, 0.0, s, c, 0.0);
}

const Transform
Translate(float tx, float ty)
{
    return Transform(1, 0, tx, 0, 1, ty);
}

const Transform
Scale(float x, float y)
{
    return Transform(x, 0.0, 0.0, 0.0, y, 0.0);
}

const Transform 
Around(const Vector & v, const Transform & t)
{
    return Translate(v.x, v.y) * t * Translate(-v.x, -v.y);
}
