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
	scene.add(new ConvexPoly(LineSegment(Vector(0.4, 0), Vector(0.4, 1), 0.01, &lscolor).transform(around(Vector(0.5,0.5),rotate(1.57)))));

	//ConvexPoly some(LineSegment(Vector(0.4, 0), Vector(0.4, 1), 0.01, &lscolor));
	Color blue(0, 0, 1, 0.5);
	//ConvexPoly tri(Triangle({ Vector(0.5, 1), Vector(0, 0), Vector(1, 0) }, &blue));
	//scene.add(&tri);
	Color green(0, 1, 0, 1);
	Ellipse circle(Circle(Vector(0.5, 0.5), 0.5, &green));

	Transform rot(rotate(3.14159 / 4));
	Transform sca(scale(1, 0.4));
	Ellipse ellip(circle.transform(around(Vector(0.5, 0.5), sca)));
	scene.add(&ellip);
	scene.draw(image);
	image.write_ppm(f);
	f.close();
}

int main()
{
	test_picture();
	return 0;
}