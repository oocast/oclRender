#include <sstream>
#include "image.h"

PPMImage::
PPMImage(size_t resolution, const Color &bg = Color()):
resolution(resolution), pixels(resolution*resolution, bg)
{
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
	oss << "P6\n" << resolution << '\n'
		<< resolution << "\n255\n";
	out << oss.str();
	for (int y = resolution - 1; y > -1; y--) {
		for (int x = 0; x < resolution; x++) {
			out << pixels[y * resolution + x].as_ppm();
		}
	}
}