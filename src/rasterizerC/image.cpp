#include <sstream>
#include "image.h"

PPMImage::
PPMImage(size_t resolution, const Color &bg):
resolution(resolution), pixels(resolution*resolution, bg)
{
}

PPMImage::
PPMImage(std::fstream &in)
{
	char _cdump;
	int _idump;
	in >> _cdump >> _cdump; // magic words, "P6" or "P3"
	in >> resolution >> resolution; //TODO: non-square
	in >> _idump; // max color value
	in.get(_cdump);
	pixels = std::vector<Color>(resolution*resolution, Color());
	for (int y = resolution - 1; y > -1; y--) {
		for (size_t x = 0; x < resolution; x++) {
			unsigned char rgbbuf[3];
			double r, g, b;
			//in >> rc >> gc >> bc;
			in.read((char *)rgbbuf, 3);
			r = (double)rgbbuf[0] / 255.0;
			g = (double)rgbbuf[1] / 255.0;
			b = (double)rgbbuf[2] / 255.0;
			pixels[y * resolution + x].fill(r, g, b);
			if (x == 0) {
				x++;
				x--;
			}
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
write_ppm(std::fstream &out, PPMImage *bg)
{
  std::ostringstream oss;
  oss << "P6 " << resolution << ' '
    << resolution << " 255\n";
  out << oss.str();
  for (int y = resolution - 1; y > -1; y--) {
    for (size_t x = 0; x < resolution; x++) {
			std::string str;
			unsigned char buf[3];
			if (x == resolution - 1) {
				x++;
				x--;
			}
			/*
			if (pixels[y * resolution + x].transparent && bg != nullptr) {
				str = bg->pixels[y * resolution + x].as_ppm();
			}
			else {
				str = pixels[y * resolution + x].as_ppm();
			}
			out << str;
			*/
			if (pixels[y * resolution + x].transparent && bg != nullptr) {
				bg->pixels[y * resolution + x].as_ppm(buf);
			}
			else {
				pixels[y * resolution + x].as_ppm(buf);
			}
			//out.put(buf[0]);
			//out.put(buf[1]);
			//out.put(buf[2]);
			out.write((char *)buf, 3);
    }
  }
}