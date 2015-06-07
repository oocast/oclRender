#ifndef __COLOR_H__
#define __COLOR_H__
#include <string>

/*
class Color
{
  float rgb[3];
  float a;
public:
	// if this pixel has been drawn
	//bool transparent;
public:
  Color(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0);
  void draw(const Color &o);
	void fill(float r, float g, float b);
  Color fainter(float k);
  std::string as_ppm();
	unsigned char* as_ppm(unsigned char *rgbbuf);
  // __repr__ operator<<
  // hex constructor
};
*/

struct Color
{
	// {r, g, b} from 0.0 to 1.0
	float rgb[3];
	// alpha value, from 0.0 to 1.0
	float a;
};

void init_Color(Color *color, float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0);
Color init_Color(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0);
void draw_Color(Color *dst, const Color *src);
Color faint_Color(const Color *color, float k);
unsigned char *as_ppm(const Color *color, unsigned char *rgbbuf);

#endif //__COLOR_H__