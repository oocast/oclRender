#include "color.h"
#include <string>
#include <cmath>

Color::
Color(double r, double g, double b, double a):
a(a)
{
  rgb[0] = r; rgb[1] = g; rgb[2] = b;
}

void Color::
draw(const Color &o)
{
  if (a == o.a && a == 0.0)
    return;
  if (o.a == 1.0) {
    rgb[0] = o.rgb[0];
    rgb[1] = o.rgb[1];
    rgb[2] = o.rgb[2];
    a = 1.0;
  }
  else {
    double u = 1.0 - o.a;
    rgb[0] = u * rgb[0] + o.a * o.rgb[0];
    rgb[1] = u * rgb[1] + o.a * o.rgb[1];
    rgb[2] = u * rgb[2] + o.a * o.rgb[2];
    a = 1.0 - (1.0 - a) * (1.0 - o.a);
  }
}

Color Color::
fainter(double k)
{
  return Color(rgb[0], rgb[1], rgb[2], a*k);
}

std::string Color::
as_ppm()
{
  std::string str;
  str += (char)(sqrt(rgb[0] * a) * 255);
  str += (char)(sqrt(rgb[1] * a) * 255);
  str += (char)(sqrt(rgb[2] * a) * 255);
	return str;
}