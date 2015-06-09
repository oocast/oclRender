#include "oclrender.h"

using namespace std;
void TestPicture()
{
    fstream f, fbg;
    f.open("test_picture.ppm", fstream::out | fstream::binary);
    fbg.open("../../pic/50608556_p0.ppm", fstream::in | fstream::binary);
    if (!fbg.is_open()) {
        exit(1);
    }
    //PPMImage image(512, InitColor(0.0, 0.0, 0.0, 1.0));
    PPMImage bg(fbg);
    fbg.close();
    Scene scene;

    // add shapes
    Color lsColor;
    InitColor(&lsColor, 1.0F, 0.0F, 0.0F, 1.0F);
    scene.Add(new ConvexPoly(LineSegment(Vector(0.0F, .0F), Vector(0.0F, 1.0F), 0.01F, &lsColor)));
    scene.Add(new ConvexPoly(LineSegment(Vector(0.1F, .0F), Vector(0.1F, 1.0F), 0.01F, &lsColor)));
    scene.Add(new ConvexPoly(LineSegment(Vector(0.2F, .0F), Vector(0.2F, 1.0F), 0.01F, &lsColor)));
    scene.Add(new ConvexPoly(LineSegment(Vector(0.3F, .0F), Vector(0.3F, 1.0F), 0.01F, &lsColor)));
    scene.Add(new ConvexPoly(LineSegment(Vector(0.4F, .0F), Vector(0.4F, 1.0F), 0.01F, &lsColor).Transformation(Around(Vector(0.5F,0.5F),Rotate(1.57F)))));

    //ConvexPoly some(LineSegment(Vector(0.4, 0), Vector(0.4, 1), 0.01, &lscolor));
    Color blue = InitColor(.0, .0, 1.0, 0.5);
    //ConvexPoly tri(Triangle({ Vector(0.5, 1), Vector(0, 0), Vector(1, 0) }, &blue));
    //scene.Add(&tri);
    Color green = InitColor(.0F, 1.0F, .0F, 1.0F);
    Color black = InitColor(.0F, .0F, .0F, 1.0F);
    Vector LL(185.0F / 512.0F, 1.0F - 205.0F / 512.0F);
    Vector CC(294.0F / 512.0F, 1.0F - 183.0F / 512.0F);
    Vector HH(403.0F / 512.0F, 1.0F - 163.0F / 512.0F);
    ConvexPoly blackBar(Rectangle(LL, HH, &black));
    ConvexPoly leanBar(blackBar.Transformation(Around(CC, Rotate(3.14159F / 13.0F))));
    scene.Add(new ConvexPoly(blackBar.Transformation(Around(CC, Rotate(3.14159F / 13.0F)))));
    Ellipse circle(Circle(Vector(0.75F, 0.1F), 0.5F, &green));

    Transform rot(Rotate(3.14159F * (1.0F / 2.0F + 1.0F / 8.0F)));
    Transform sca(Scale(1.0F, 0.2F));
    //Ellipse ellip(circle.Transformation(Around(Vector(0.75F, 0.1F), rot * sca)));
    scene.Add(new Ellipse(circle.Transformation(Around(Vector(0.75F, 0.1F), rot * sca))));

    //ConvexPoly tri1(Triangle({ Vector(0.25, 0.75), Vector(0.25, 0.5), Vector(0.5, 0.75) }, &green));
    //ConvexPoly tri2(Triangle({ Vector(0.5, 0.5), Vector(0.5, 0.75), Vector(0.25, 0.5) }, &green));
    //scene.Add(new ConvexPoly(tri1));
    //scene.Add(new ConvexPoly(tri2));

    char fileName[]="shapedraw.cl";
    char kernelName[]="ShapeDraw";
    
    CLInit(fileName, kernelName);

    CLReadImageBuff(memObj[0], bg);
    //image_modifyio(scene, bg);
    scene.Draw(bg);
    CLWriteImageBuff(memObj[0], bg);
    /*
    int size=bg.width*bg.height*sizeof(Color);
    memObj[0]=clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR, size, &bg.pixels[0],NULL);

    scene.draw(bg);

    err=clEnqueueReadBuffer(cmd_queue, memObj[0], CL_TRUE, 0, size, &bg.pixels[0], 0, NULL, NULL);
    */

    CLRelease();
    //image.write_ppm(f, &bg);
    bg.WritePPM(f);
    f.close();
}

int main()
{
    TestPicture();
    return 0;
}
