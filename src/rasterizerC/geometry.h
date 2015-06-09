#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

// *x stores both roots of the quadratic equation
// x: an array of float with size 2
void quadratic(float a, float b, float c,
        float *x);

class Vector
{
public:
  float x, y;
  Vector(float x_in = 0.0, float y_in = 0.0);
  // TODO: operator overload
  Vector operator+(const Vector &o) const;
  Vector operator-(const Vector &o) const;
  Vector operator*(float k) const;

  float dot(const Vector &o) const;
  Vector min(const Vector &o) const;
  Vector max(const Vector &o) const;
  float length();
  void repr(); // TODO: what's the meaning?
};

class AABox
{
public:
  Vector Low, High; // use Vector to represent points
public:
  AABox(const Vector &Point1 = Vector(), const Vector &Point2 = Vector());
  Vector midpoint() const;
  Vector size() const;
  bool contains(const Vector &Point) const;
  bool overlaps(const AABox &Other) const;
  AABox intersection(const AABox &Other) const;
  static AABox fromVectors(const Vector *Vectors, const int VectorNum);
};

class HalfPlane
{
public:
  Vector ab;
  float c;
public:
  HalfPlane(const Vector &Point1, const Vector &Point2);
  float signedDistance(const Vector &Point) const;
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
  const Transform operator*(const Transform &other) const;
  const Vector operator*(const Vector &other) const;
  float det() const;
  Transform inverse() const;
};

const Transform identity();
const Transform rotate(float Theta);
const Transform translate(float tx, float ty);
const Transform scale(float x, float y);
const Transform around(const Vector &v, const Transform &t);

#endif //__GEOMETRY_H__