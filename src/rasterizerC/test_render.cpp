#include "oclrender.h"
#include <fstream>

using namespace std;
void test_picture()
{
	fstream f;
	f.open("test_picture.ppm", fstream::out | fstream::binary);
	PPMImage image = PPMImage(512, Color(1, 1, 1, 1));
	Scene scene;

	// add shapes
	Color lscolor(1, 0, 0, 1);
	scene.add(new ConvexPoly(LineSegment(Vector(0.0, 0), Vector(0.0, 1), 0.01, &lscolor)));
	scene.add(new ConvexPoly(LineSegment(Vector(0.1, 0), Vector(0.1, 1), 0.01, &lscolor)));
	scene.add(new ConvexPoly(LineSegment(Vector(0.2, 0), Vector(0.2, 1), 0.01, &lscolor)));
	scene.add(new ConvexPoly(LineSegment(Vector(0.3, 0), Vector(0.3, 1), 0.01, &lscolor)));
	scene.add(new ConvexPoly(LineSegment(Vector(0.4, 0), Vector(0.4, 1), 0.01, &lscolor)));

	scene.draw(image);
	image.write_ppm(f);
	f.close();
}

int main()
{
	test_picture();
	return 0;
}