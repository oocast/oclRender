#include <random>
#include <cmath>
#include "geometry.h"

void 
quadratic(float a, float b, float c,
        float *x)
{
  if (a == 0.0) {
    x[0] = -c/b;
    x[1] = -c/b;
  }
  float d = sqrt(b * b - 4 * a * c);	
  if (b >= 0) {
    x[0] = (-b - d) / (2 * a);
    x[1] = (2 * c) / (-b - d);
  }
  else {
    x[0] = (2 * c) / (-b + d);
    x[1] = (-b + d) / (2 * a);
  }
}

Vector::
Vector(float x_in, float y_in):
x(x_in), y(y_in)
{}

Vector Vector::
operator+(const Vector &o) const
{
  return Vector(x + o.x, y + o.y);
}

Vector Vector::
operator-(const Vector &o) const
{
  return Vector(x - o.x, y - o.y);
}

Vector Vector::
operator*(float k) const
{
  return Vector(x * k, y * k);
}

float Vector::
dot(const Vector &o) const
{
  return x * o.x + y * o.y;
}

Vector Vector::
min(const Vector &o) const
{
  return Vector(fmin(x, o.x), fmin(y, o.y));
}

Vector Vector::
max(const Vector &o) const
{
  return Vector(fmax(x, o.x), fmax(y, o.y));
}

float Vector::
length()
{
  return sqrt(x * x + y * y);
}

AABox::
AABox(const Vector &p1, const Vector &p2):
Low(p1.min(p2)), High(p2.max(p2))
{}

Vector AABox::
midpoint() const
{
  return (Low + High) * 0.5;
}

Vector AABox::
size() const
{
  return High - Low;
}

bool AABox::
contains(const Vector &Point) const
{
  return (Low.x <= Point.x) &&
      (Point.x <= High.x) &&
      (Low.y <= Point.y) &&
      (Point.y <= High.y);
}

bool AABox::
overlaps(const AABox &Other) const
{
  return !((Other.Low.x >= High.x) ||
      (Other.High.x <= Low.x) ||
      (Other.Low.y >= High.y) ||
      (Other.High.y <= Low.y));
}

AABox AABox::
intersection(const AABox &Other) const
{
  return AABox(Low.max(Other.Low), High.min(Other.High));
}

AABox AABox::
fromVectors(const Vector *Vectors, const int VectorNum)
{
  Vector tmp_low = Vectors[0], tmp_high = Vectors[0];
  for (int i = 1; i < VectorNum; i++) {
    tmp_low = tmp_low.min(Vectors[i]);
    tmp_high = tmp_high.max(Vectors[i]);
  }
  return AABox(tmp_low, tmp_high);
}

HalfPlane::
HalfPlane(const Vector &Point1, const Vector &Point2):
ab(-Point2.y + Point1.y, Point2.x - Point1.x)
{
  float l = ab.length();
  c = -ab.dot(Point1) / l;
  ab = ab * (1.0F/ l);
}

float HalfPlane::
signedDistance(const Vector &Point) const
{
  return ab.dot(Point) + c;
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
operator*(const Transform &other) const
{
  float t[2][3] = {{0.0, 0.0, 0.0},
          {0.0, 0.0, 0.0}};	
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        t[i][j] += m[i][k] * other.m[k][j];
      }
    }
  }
  return Transform(t[0][0], t[0][1], t[0][2],
          t[1][0], t[1][1], t[1][2]);
}

const Vector Transform::
operator*(const Vector &other) const
{
  float nx = m[0][0] * other.x + m[0][1] * other.y + m[0][2];
  float ny = m[1][0] * other.x + m[1][1] * other.y + m[1][2];
  return Vector(nx, ny);
}

float Transform::
det() const
{
  return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

Transform Transform::
inverse() const
{
  float d = 1.0F / det();
  Transform t = Transform(d * m[1][1], -d * m[0][1], 0, 
              -d * m[1][0], d * m[0][0], 0);
  Vector v = t * Vector(m[0][2], m[1][2]);
  t.m[0][2] = -v.x;
  t.m[1][2] = -v.y;
  return t;
}

const Transform
identity()
{
  return Transform(1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

const Transform
rotate(float Theta)
{
  float s = sin(Theta);
  float c = cos(Theta);
  return Transform(c, -s, 0.0, s, c, 0.0);
}

const Transform
translate(float tx, float ty)
{
  return Transform(1, 0, tx, 0, 1, ty);
}

const Transform
scale(float x, float y)
{
  return Transform(x, 0.0, 0.0, 0.0, y, 0.0);
}

const Transform 
around(const Vector &v, const Transform &t)
{
  return translate(v.x, v.y) * t * translate(-v.x, -v.y);
}
