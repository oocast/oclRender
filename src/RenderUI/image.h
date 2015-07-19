#ifndef __IMAGE_H__
#define __IMAGE_H__
#include <fstream>	// fstream
#include <vector>

#include "color.h"
#include "geometry.h"

class PPMImage
{
public:
    size_t width, height;
    std::vector<Color> pixels;
public:
    PPMImage(size_t, size_t, const Color & backgroundColor = Color());
    // TODO: support non-square input PPM image
    PPMImage(std::fstream &);
    AABox Bounds() const;
    void WritePPM(std::fstream &);
    //Color& operator[](const Vector &a); // left value
    //const Color& operator[](const Vector &a) const; // right value
};

#endif //__IMAGE_H__
