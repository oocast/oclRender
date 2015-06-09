#ifndef __IMAGE_H__
#define __IMAGE_H__
#include <fstream>	// fstream
#include <vector>
#include "color.h"
#include "geometry.h"

class PPMImage
{
public:
  size_t Width, Height;
  std::vector<Color> Pixels;
public:
  PPMImage(size_t Width, size_t Height, const Color &BackgroundColor = Color());
  // TODO: support non-square input PPM image
  PPMImage(std::fstream &FileIn);
  AABox bounds() const;
  void writePPM(std::fstream &FileOut);
  //Color& operator[](const Vector &a); // left value
  //const Color& operator[](const Vector &a) const; // right value
};

#endif //__IMAGE_H__