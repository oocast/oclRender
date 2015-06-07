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
low(p1.min(p2)), high(p2.max(p2))
{}

Vector AABox::
midpoint() const
{
  return (low + high) * 0.5;
}

Vector AABox::
size() const
{
  return high - low;
}

bool AABox::
contains(const Vector &p) const
{
  return (low.x <= p.x) &&
      (p.x <= high.x) &&
      (low.y <= p.y) &&
      (p.y <= high.y);
}

bool AABox::
overlaps(const AABox &r) const
{
  return !((r.low.x >= high.x) ||
      (r.high.x <= low.x) ||
      (r.low.y >= high.y) ||
      (r.high.y <= low.y));
}

AABox AABox::
intersection(const AABox &other) const
{
  return AABox(low.max(other.low), high.min(other.high));
}

AABox AABox::
from_vectors(const Vector *vcts, const int vnum)
{
  Vector tmp_low = vcts[0], tmp_high = vcts[0];
  for (int i = 1; i < vnum; i++) {
    tmp_low = tmp_low.min(vcts[i]);
    tmp_high = tmp_high.max(vcts[i]);
  }
  return AABox(tmp_low, tmp_high);
}

HalfPlane::
HalfPlane(const Vector &p1, const Vector &p2):
v(-p2.y + p1.y, p2.x - p1.x)
{
  float l = v.length();
  c = -v.dot(p1) / l;
  v = v * (1.0F/ l);
}

float HalfPlane::
signed_distance(const Vector p) const
{
  return v.dot(p) + c;
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
rotate(float theta)
{
  float s = sin(theta);
  float c = cos(theta);
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
