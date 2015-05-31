#include "color.h"
#include <string>
#include <cmath>

Color::
Color(double r, double g, double b, double a) :
a(a), transparent(true)
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
	transparent = false;
}

void Color::
fill(double r, double g, double b)
{
	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
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
	unsigned char r, g, b;
	if (a == 1.0) {
		r = (unsigned char)(rgb[0] * 255);
		g = (unsigned char)(rgb[1] * 255);
		b = (unsigned char)(rgb[2] * 255);
	}
	else {
		r = (unsigned char)(sqrt(rgb[0] * a) * 255);
		g = (unsigned char)(sqrt(rgb[1] * a) * 255);
		b = (unsigned char)(sqrt(rgb[2] * a) * 255);
	}
	str += r;
	str += g;
	str += b;
	return str;
}

unsigned char* Color::
as_ppm(unsigned char *rgbbuf)
{
	unsigned char r, g, b;
	if (a == 1.0) {
		r = (unsigned char)(rgb[0] * 255);
		g = (unsigned char)(rgb[1] * 255);
		b = (unsigned char)(rgb[2] * 255);
	}
	else {
		r = (unsigned char)(sqrt(rgb[0] * a) * 255);
		g = (unsigned char)(sqrt(rgb[1] * a) * 255);
		b = (unsigned char)(sqrt(rgb[2] * a) * 255);
	}
	rgbbuf[0] = r; rgbbuf[1] = g; rgbbuf[2] = b;
	return rgbbuf;
}