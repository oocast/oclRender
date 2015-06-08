#include "oclrender.h"
#include "cl_helper.h"
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace std;
void test_picture()
{
	fstream f, fbg;
	f.open("test_picture.ppm", fstream::out | fstream::binary);
	fbg.open("../../pic/50608556_p0.ppm", fstream::in | fstream::binary);
	if (!fbg.is_open()) {
		exit(1);
	}
	//PPMImage image(512, init_Color(0.0, 0.0, 0.0, 1.0));
	PPMImage bg(fbg);
	fbg.close();
	Scene scene;

	// add shapes
	Color lscolor;
	init_Color(&lscolor, 1.0F, 0.0F, 0.0F, 1.0F);
	scene.add(new ConvexPoly(LineSegment(Vector(0.0F, .0F), Vector(0.0F, 1.0F), 0.01F, &lscolor)));
	scene.add(new ConvexPoly(LineSegment(Vector(0.1F, .0F), Vector(0.1F, 1.0F), 0.01F, &lscolor)));
	scene.add(new ConvexPoly(LineSegment(Vector(0.2F, .0F), Vector(0.2F, 1.0F), 0.01F, &lscolor)));
	scene.add(new ConvexPoly(LineSegment(Vector(0.3F, .0F), Vector(0.3F, 1.0F), 0.01F, &lscolor)));
	scene.add(new ConvexPoly(LineSegment(Vector(0.4F, .0F), Vector(0.4F, 1.0F), 0.01F, &lscolor).transform(around(Vector(0.5F,0.5F),rotate(1.57F)))));

	//ConvexPoly some(LineSegment(Vector(0.4, 0), Vector(0.4, 1), 0.01, &lscolor));
	Color blue = init_Color(.0, .0, 1.0, 0.5);
	//ConvexPoly tri(Triangle({ Vector(0.5, 1), Vector(0, 0), Vector(1, 0) }, &blue));
	//scene.add(&tri);
	Color green = init_Color(.0F, 1.0F, .0F, 1.0F);
	Color black = init_Color(.0F, .0F, .0F, 1.0F);
	Vector LL(185.0F / 512.0F, 1.0F - 205.0F / 512.0F);
	Vector CC(294.0F / 512.0F, 1.0F - 183.0F / 512.0F);
	Vector HH(403.0F / 512.0F, 1.0F - 163.0F / 512.0F);
	ConvexPoly blackbar(Rectangle(LL, HH, &black));
	ConvexPoly leanbar(blackbar.transform(around(CC, rotate(3.14159F / 13.0F))));
	scene.add(new ConvexPoly(blackbar.transform(around(CC, rotate(3.14159F / 13.0F)))));
	Ellipse circle(Circle(Vector(0.75F, 0.1F), 0.5F, &green));

	Transform rot(rotate(3.14159F * (1.0F / 2.0F + 1.0F / 8.0F)));
	Transform sca(scale(1.0F, 0.2F));
	//Ellipse ellip(circle.transform(around(Vector(0.75F, 0.1F), rot * sca)));
	scene.add(new Ellipse(circle.transform(around(Vector(0.75F, 0.1F), rot * sca))));

	//ConvexPoly tri1(Triangle({ Vector(0.25, 0.75), Vector(0.25, 0.5), Vector(0.5, 0.75) }, &green));
	//ConvexPoly tri2(Triangle({ Vector(0.5, 0.5), Vector(0.5, 0.75), Vector(0.25, 0.5) }, &green));
	//scene.add(new ConvexPoly(tri1));
	//scene.add(new ConvexPoly(tri2));

	char fileName[]="kernel.cl";
	char kernelName[]="drawKernel";
	
	init(fileName, kernelName);

	int size=bg.width*bg.height*sizeof(Color);
	memObj[0]=clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR, size, &bg.pixels[0],NULL);

	scene.draw(bg);

	err=clEnqueueReadBuffer(cmd_queue, memObj[0], CL_TRUE, 0, size, &bg.pixels[0], 0, NULL, NULL);

	release();
	//image.write_ppm(f, &bg);
	bg.write_ppm(f);
	f.close();
}

int main()
{
	test_picture();
	return 0;
}
