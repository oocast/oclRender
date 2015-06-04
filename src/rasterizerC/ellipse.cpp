#include <stdexcept>
#include <cmath>
#include "ellipse.h"

Ellipse::
Ellipse(float a, float b, float c,
      float d, float e, float f,
      const Color *color):
Shape(color), a(a), b(b), c(c),
d(d), e(e), f(f)
{
  if (c*c - 4*a*b >= 0) {
    throw std::invalid_argument("Not an ellipse");
  }
  gradient = Transform(2*a, c, d, c, 2*b, e);
  center = gradient.inverse() * Vector(0.0, 0.0);
  float x[2], y[2];
  quadratic(b-c*c/(4*a), e-c*d/(2*a), f-d*d/(4*a), y);
  quadratic(a-c*c/(4*b), d-c*e/(2*b), f-e*e/(4*b), x);
  // y bounds: dv/dx(x, y) = 0, v(x,y) = 0
  Vector boundVectors[4] = {
    Vector(-(d + c*y[0])/(2*a), y[0]),
    Vector(-(d + c*y[1])/(2*a), y[1]),
    Vector(x[0], -(e + c*x[0])/(2*b)),
    Vector(x[1], -(e + c*x[1])/(2*b))
  };
  bound = AABox::from_vectors(boundVectors, 4);
  if (!contains(center)) {
    throw std::logic_error("Internal error, center not inside ellipse");
  }
}

//Ellipse::Ellipse(const Ellipse &&ellipse){}

float Ellipse::
value(const Vector &p) const
{
  return (a*p.x*p.x + b*p.y*p.y + c*p.x*p.y +
      d*p.x + e*p.y + f);
}

bool Ellipse::
contains(const Vector &p) const
{
  return value(p) < 0;
}

Ellipse Ellipse::
transform(const Transform &transform)
{
  Transform i(transform.inverse());
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
  return Ellipse(aa, bb, cc, dd, ee, ff, &color);
}

void Ellipse::
intersections(const Vector &c, const Vector &p, Vector *inter_ps) const
{
  // returns the two intersections of the line through c and p
    // and the ellipse. Defining a line as a function of a single
    // parameter u, x(u) = c.x + u * (p.x - c.x), (and same for y)
    // this simply solves the quadratic equation f(x(u), y(u)) = 0
  Vector pc = p - c;
  float u2, u1, u0;
  u2 = a*pc.x*pc.x + b*pc.y*pc.y + this->c*pc.x*pc.y;
  u1 = 2* a*c.x*pc.x + 2* b*c.y*pc.y 
        + this->c*c.y*pc.x + this->c*c.x*pc.y + d*pc.x 
        + e*pc.y;
  u0 =  a*c.x*c.x + b*c.y*c.y + this->c*c.x*c.y 
        + d*c.x + e*c.y + f;
  /*
  // try catch block for imaginary solutions
  try{
  }
  catch{
  }
  */
  float sols[2];
  quadratic(u2, u1, u0, sols);
  inter_ps[0] = c+pc*sols[0];
  inter_ps[1] = c+pc*sols[1];
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
get_parameters(std::vector<float> &paras, ShapeType *shapeType)
{
	paras.push_back(a);
	paras.push_back(b);
	paras.push_back(c);
	paras.push_back(d);
	paras.push_back(e);
	paras.push_back(f);
	*shapeType = ELLIPSE;
}

Ellipse 
Circle(const Vector &center, float radius, const Color *color)
{
  return Ellipse(1.0, 1.0, 0.0, 0.0, 0.0, -1.0, color).transform(
    scale(radius, radius)).transform(
    translate(center.x, center.y));
}