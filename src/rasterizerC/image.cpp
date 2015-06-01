#include <sstream>
#include "image.h"

PPMImage::
PPMImage(size_t width, size_t height, const Color &bg):
width(width), height(height), pixels(width*height, bg)
{
}

PPMImage::
PPMImage(std::fstream &in)
{
	char _cdump;
	int _idump;
	in >> _cdump >> _cdump; // magic words, "P6" or "P3"
	in >> width >> height; //TODO: non-square
	in >> _idump; // max color value
	in.get(_cdump);
	pixels = std::vector<Color>(width*height, Color());
	for (int y = height - 1; y > -1; y--) {
		for (size_t x = 0; x < width; x++) {
			unsigned char rgbbuf[3];
			float r, g, b;
			//in >> rc >> gc >> bc;
			in.read((char *)rgbbuf, 3);
			r = (float)rgbbuf[0] / 255.0F;
			g = (float)rgbbuf[1] / 255.0F;
			b = (float)rgbbuf[2] / 255.0F;
			init_Color(&pixels[y * width + x], r, g, b);
			if (in.eof()) {
				//in.close();
				//exit(1);
			}
		}
	}
}

AABox PPMImage:: 
bounds() const
{
  return AABox(Vector(0.0, 0.0), Vector(1.0, 1.0));	
}

void PPMImage::
write_ppm(std::fstream &out)
{
  std::ostringstream oss;
  oss << "P6 " << width << ' '
    << height << " 255\n";
  out << oss.str();
  for (int y = height - 1; y > -1; y--) {
    for (size_t x = 0; x < width; x++) {
			std::string str;
			unsigned char buf[3];
			/*
			if (pixels[y * resolution + x].transparent && bg != nullptr) {
				str = bg->pixels[y * resolution + x].as_ppm();
			}
			else {
				str = pixels[y * resolution + x].as_ppm();
			}
			out << str;
			*/
			as_ppm(&pixels[y * width + x], buf);
			//out.put(buf[0]);
			//out.put(buf[1]);
			//out.put(buf[2]);
			out.write((char *)buf, 3);
    }
  }
}