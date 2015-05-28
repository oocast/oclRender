#include <random>
#include <vector>
#include <cmath>
#include "color.h"
#include "geometry.h"
#include "shape.h"

Shape::
Shape(const Color *o_color = null_ptr)
{
	if (o_color != null_ptr) {
		color = *o_color;
	}
}

void Shape::
draw(PPMImage &image, int super_sampling)
{
	if (!bound.overlaps(image.bounds())) {
		return;
	}
	float r = (float)image.resolution;
	std::vector<Vector> jitter;
	std::default_random_engine gen;
	std::uniform_real_distribution<double> distri(0.0, 1.0);
	for (int x = 0; x < super_sampling; x++) {
		for (int y = 0; y < super_sampling; y++) {
			jitter[x * super_sampling + y] = Vector( ((double)x + distri(gen)) / super_sampling / r,
				((double)y + distri(gen)) / super_sampling / r);
		}
	}
	size_t lj = jitter.size();
	int l_x, l_y, h_x, h_y;
	l_x = (int)fmax(bound.low.x * r, 0);
	l_y = (int)fmax(bound.low.y * r, 0);
	h_x = (int)fmin(bound.high.x * r, r-1);
	h_y = (int)fmin(bound.high.y * r, r-1);
	for (int y = l_y; y < h_y; y++) {
		int x = l_x;
		while (x <= h_x) {
			Vector corner = Vector((double)x / r, (double)y / r);
			double b = signed_distance_bound(corner);
			double pixel_diameter = sqrt(2.0) / r;
			if (b > pixel_diameter) {
				int steps = (int)(r * (b - pixel_diameter + 1.0 / r));
				int xend = (int)fmin(x + steps, h_x + 1);
				for (int x_ = x; x_ < xend; x_++) {
					image.pixels[y * image.resolution + x_].draw(color);
				}
				x += steps;
			}
			else if (b < -pixel_diameter) {
				int steps = (int)(r * (-b - pixel_diameter + 1.0 / r));
				x += steps;
			}
			else {
				double coverage = 0.0;
				for (int i = 0; i < lj; i++) {
					if (contains(corner + jitter[i])) {
						coverage += 1.0;
					}
				}
				image.pixels[y * image.resolution + x].draw(color.fainter(coverage / lj));
				x++;
			}
		}
	}
}