#include "color.h"
#include <string>
#include <cmath>
#include <cstring>

/*
Color::
Color(float r, float g, float b, float a) :
a(a)
{
  rgb[0] = r; rgb[1] = g; rgb[2] = b;
}

void Color::
draw(const Color &o)
{
  if (a == o.a && a == 0.0)
    return;
  float u = 1.0 - o.a;
  rgb[0] = u * rgb[0] + o.a * o.rgb[0];
  rgb[1] = u * rgb[1] + o.a * o.rgb[1];
  rgb[2] = u * rgb[2] + o.a * o.rgb[2];
  a = 1.0 - (1.0 - a) * (1.0 - o.a);
}

void Color::
fill(float r, float g, float b)
{
	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

Color Color::
fainter(float k)
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
*/

void
init_Color(Color *color, float r, float g, float b, float a)
{
	color->rgb[0] = r;
	color->rgb[1] = g;
	color->rgb[2] = b;
	color->a = a;
}

Color
init_Color(float r, float g, float b, float a)
{
	Color result;
	result.rgb[0] = r;
	result.rgb[1] = g;
	result.rgb[2] = b;
	result.a = a;
	return result;
}

void
draw_Color(Color *dst, const Color *src)
{
	if (dst->a == src->a && dst->a == 0.0)
		return;
	float u = 1.0F - src->a;
	dst->rgb[0] = u * dst->rgb[0] + src->a * src->rgb[0];
	dst->rgb[1] = u * dst->rgb[1] + src->a * src->rgb[1];
	dst->rgb[2] = u * dst->rgb[2] + src->a * src->rgb[2];
	dst->a = 1.0F - (1.0F - dst->a) * (1.0F - src->a);
}

Color
faint_Color(const Color *color, float k)
{
	Color result;
	memcpy(&result, color, sizeof(Color));
	result.a *= k;
	return result;
}

unsigned char *
as_ppm(const Color *color, unsigned char *rgbbuf)
{
	unsigned char r, g, b;
	r = (unsigned char)(color->rgb[0] * color->a * 255);
	g = (unsigned char)(color->rgb[1] * color->a * 255);
	b = (unsigned char)(color->rgb[2] * color->a * 255);
	rgbbuf[0] = r; rgbbuf[1] = g; rgbbuf[2] = b;
	return rgbbuf;
}