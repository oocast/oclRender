#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

// *x stores both roots of the quadratic equation
// x: an array of float with size 2
void Quadratic(float, float, float, float *);

class Vector
{
public:
    float x, y;
    Vector(float xIn = 0.0, float yIn = 0.0);
    // TODO: operator overload
    Vector operator+(const Vector &) const;
    Vector operator-(const Vector &) const;
    Vector operator*(float) const;

    float Dot(const Vector &) const;
    Vector Min(const Vector &) const;
    Vector Max(const Vector &) const;
    float Length();
    void Repr(); // TODO: what's the meaning?
};

class AABox
{
public:
    Vector low, high; // use Vector to represent points
public:
    AABox(const Vector &point1 = Vector(), const Vector &point2 = Vector());
    Vector Midpoint() const;
    Vector Size() const;
    bool Contains(const Vector &) const;
    bool Overlaps(const AABox &) const;
    AABox Intersection(const AABox &) const;
    static AABox FromVectors(const Vector *, const int);
};

class HalfPlane
{
public:
    Vector ab;
    float c;
public:
    HalfPlane(const Vector &, const Vector &);
    float SignedDistance(const Vector &) const;
};

// Transform represents an affine transformation
// of a 2D vector ("affine" because it's a linear transformation
// combined with a translation)
class Transform
{
public:
    float m[3][3];
public:
    Transform(float m11 = 1.0, float m12 = 0.0, float tx = 0.0, 
              float m21 = 0.0, float m22 = 1.0, float ty = 0.0);
    const Transform operator*(const Transform &) const;
    const Vector operator*(const Vector &) const;
    float Det() const;
    Transform Inverse() const;
};

const Transform Identity();
const Transform Rotate(float);
const Transform Translate(float, float);
const Transform Scale(float, float);
const Transform Around(const Vector &, const Transform &);

#endif //__GEOMETRY_H__
