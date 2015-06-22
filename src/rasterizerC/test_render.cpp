#include "oclrender.h"

#include<iostream>

const float PI=3.14159f;

using namespace std;
void TestPicture(char * outName)
{
    fstream f, fbg;
    f.open(outName, fstream::out | fstream::binary);
    fbg.open("../../pic/50608556_p0.ppm", fstream::in | fstream::binary);
    if (!fbg.is_open())
        exit(1);
    //PPMImage image(512, InitColor(0.0, 0.0, 0.0, 1.0));
    PPMImage bg(fbg);
    fbg.close();
    Scene scene;
/*
    // add shapes
    Color lsColor;
    InitColor(&lsColor, 1.0F, 0.0F, 0.0F, 1.0F);
    scene.Add(new ConvexPoly(LineSegment(Vector(0.0F, .0F), Vector(0.0F, 1.0F), 0.01F, &lsColor)));
    scene.Add(new ConvexPoly(LineSegment(Vector(0.1F, .0F), Vector(0.1F, 1.0F), 0.01F, &lsColor)));
    scene.Add(new ConvexPoly(LineSegment(Vector(0.2F, .0F), Vector(0.2F, 1.0F), 0.01F, &lsColor)));
    scene.Add(new ConvexPoly(LineSegment(Vector(0.3F, .0F), Vector(0.3F, 1.0F), 0.01F, &lsColor)));
    ConvexPoly * vertical=new ConvexPoly(LineSegment(Vector(0.4F, .0F), Vector(0.4F, 1.0F), 0.01F, &lsColor));
    vertical->Transformation(Around(Vector(0.5F,0.5F),Rotate(PI /2)));
    scene.Add(vertical);

    //ConvexPoly some(LineSegment(Vector(0.4, 0), Vector(0.4, 1), 0.01, &lscolor));
    Color blue = InitColor(.0, .0, 1.0, 0.5);
    for (int k=0; k<100; k++) scene.Add(new ConvexPoly(LineSegment(Vector(0.49F, 0.08F), Vector(0.49F, 0.14F), 0.06F, &blue)));
    //ConvexPoly tri(Triangle({ Vector(0.5, 1), Vector(0, 0), Vector(1, 0) }, &blue));
    //scene.Add(&tri);
    Color green = InitColor(.0F, 1.0F, .0F, 1.0F);
    Color black = InitColor(.0F, .0F, .0F, 1.0F);
    Vector LL(185.0F / 512.0F, 1.0F - 205.0F / 512.0F);
    Vector CC(294.0F / 512.0F, 1.0F - 183.0F / 512.0F);
    Vector HH(403.0F / 512.0F, 1.0F - 163.0F / 512.0F);
    ConvexPoly * blackBar=new ConvexPoly(Rectangle(LL, HH, &black));
    blackBar->Transformation(Around(CC, Rotate(PI / 13.0F)));
    //ConvexPoly leanBar(blackBar.Transformation(Around(CC, Rotate(PI / 13.0F))));
    scene.Add(blackBar);
    Ellipse * circle=new Ellipse(Circle(Vector(0.75F, 0.1F), 0.5F, &green));

    Transform rot(Rotate(PI * (1.0F / 2.0F + 1.0F / 8.0F)));
    Transform sca(Scale(1.0F, 0.2F));
    circle->Transformation(Around(Vector(0.75F, 0.1F), rot * sca));
    //Ellipse ellip(circle.Transformation(Around(Vector(0.75F, 0.1F), rot * sca)));
    scene.Add(circle);	

    //ConvexPoly tri1(Triangle({ Vector(0.25, 0.75), Vector(0.25, 0.5), Vector(0.5, 0.75) }, &green));
    //ConvexPoly tri2(Triangle({ Vector(0.5, 0.5), Vector(0.5, 0.75), Vector(0.25, 0.5) }, &green));
    //scene.Add(new ConvexPoly(tri1));
    //scene.Add(new ConvexPoly(tri2));
*/
    int numUnion, positive, numIntersect, numEle, id;
    Color color;
    std::cin>>numUnion;
    std::shared_ptr<Shape> sp;
    for (int i=0; i<numUnion; i++)
    {
        std::cin>>color.rgb[0]>>color.rgb[1]>>color.rgb[2]>>color.a;
        std::cin>>positive>>numIntersect;
        Union * up=new Union(&color, positive);
        for (int j=0; j<numIntersect; j++)
        {
            //std::cin>>color.rgb[0]>>color.rgb[1]>>color.rgb[2]>>color.a;
            std::cin>>positive>>numEle;
            Intersection * ip=new Intersection(NULL, positive);
            for (int k=0; k<numEle; k++)
            {
                //std::cin>>color.rgb[0]>>color.rgb[1]>>color.rgb[2]>>color.a;
                std::cin>>positive>>id;
                if (id==0)
                {
                    float a, b, c, d, e, f;
                    std::cin>>a>>b>>c>>d>>e>>f;
                    Ellipse * ep=new Ellipse(a, b, c, d, e, f, NULL, positive);
                    sp=(std::shared_ptr<Shape>) ep;
                }
                else
                {
                    Vector vt;
                    vector<Vector> vv;
                    for (int l=0; l<id; l++)
                    {
                        std::cin>>vt.x>>vt.y;
                        vv.push_back(vt);
                    }
                    ConvexPoly * cp=new ConvexPoly(vv, NULL, positive);
                    sp=(std::shared_ptr<Shape>) cp;
                }
                ip->AddElement(sp);
            }
            sp=(std::shared_ptr<Shape>) ip;
            up->AddElement(sp);
        }
        //sp=(std::shared_ptr<Shape>) up;
        scene.Add(up);
    }
/*
    for (int i=0; i<10; i++)
    {
        for (int j=0; j<10; j++)
        {
            Translate(0.05+0.1*i, 0.05+0.1*j);
*/            

    scene.Draw(bg);

    //image.write_ppm(f, &bg);
    bg.WritePPM(f);
    f.close();

}

int main()
{
    char outName[]="./result/case000";
    int n;
    std::cin>>n;
    for (int i=0; i<n; i++)
    {
        outName[15]=48+i%10;
        outName[14]=48+(i/10)%10;
        outName[13]=48+i/100;
        std::cout<<"Case: "<<i<<" : "<<std::flush;
        TestPicture(outName);
    }
    return 0;
}
