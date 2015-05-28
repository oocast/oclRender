#ifndef __IMAGE_H__
#define __IMAGE_H__
#include <fstream>	// fstream
#include <vector>
#include "color.h"
#include "geometry.h"

class PPMImage
{
public:
	size_t resolution;
	std::vector<Color> pixels;
public:
	PPMImage(size_t resolution, const Color &bg = Color());
	AABox bounds() const;
	void write_ppm(std::fstream &out);
	//Color& operator[](const Vector &a); // left value
	//const Color& operator[](const Vector &a) const; // right value
};

#endif //__IMAGE_H__