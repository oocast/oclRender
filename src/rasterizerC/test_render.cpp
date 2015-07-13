#include "oclrender.h"

#include<iostream>
#include<cmath>

const float PI=3.14159f;
const float StarR=0.05f;

using namespace std;

void TestStarUnions(char * outName, int nnn, int abc)
{
    fstream f, fbg;
    f.open(outName, fstream::out | fstream::binary);
    fbg.open("../../pic/50608556_p0.ppm", fstream::in | fstream::binary);
    //fbg.open("../../pic/abc.ppm", fstream::in | fstream::binary);
    if (!fbg.is_open())
        exit(1);
    //PPMImage image(512, InitColor(0.0, 0.0, 0.0, 1.0));
    PPMImage bg(fbg);
    fbg.close();
    Scene scene;

    Color color;
    InitColor(&color, 1, 0, 0, 1);
///*
    std::vector<Vector> vv1, vv2, vv3, vv4;
    std::shared_ptr<Shape> sp;

    vv1.push_back(Vector(sin(0)*StarR, cos(0)*StarR));
    vv1.push_back(Vector(sin(PI*6/5)*StarR, cos(PI*6/5)*StarR));
    vv1.push_back(Vector(sin(PI*4/5)*StarR, cos(PI*4/5)*StarR));
    vv2.push_back(Vector(sin(PI/5)*StarR*sin(PI/10)/cos(PI/5), cos(PI/5)*StarR*sin(PI/10)/cos(PI/5)));
    vv2.push_back(Vector(sin(PI*3/5)*StarR*sin(PI/10)/cos(PI/5), cos(PI*3/5)*StarR*sin(PI/10)/cos(PI/5)));
    vv2.push_back(Vector(sin(PI*2/5)*StarR, cos(PI*2/5)*StarR));
    vv3.push_back(Vector(sin(PI*7/5)*StarR*sin(PI/10)/cos(PI/5), cos(PI*7/5)*StarR*sin(PI/10)/cos(PI/5)));
    vv3.push_back(Vector(sin(PI*9/5)*StarR*sin(PI/10)/cos(PI/5), cos(PI*9/5)*StarR*sin(PI/10)/cos(PI/5)));
    vv3.push_back(Vector(sin(PI*8/5)*StarR, cos(PI*8/5)*StarR));
    vv4.push_back(Vector(sin(PI)*StarR*sin(PI/10)/cos(PI/5), cos(PI)*StarR*sin(PI/10)/cos(PI/5)));
    vv4.push_back(Vector(sin(PI*6/5)*StarR, cos(PI*6/5)*StarR));
    vv4.push_back(Vector(sin(PI*4/5)*StarR, cos(PI*4/5)*StarR));
    
    ConvexPoly * cp1=new ConvexPoly(vv1, NULL, 1);
    ConvexPoly * cp2=new ConvexPoly(vv2, NULL, 1);
    ConvexPoly * cp3=new ConvexPoly(vv3, NULL, 1);
    ConvexPoly * cp4=new ConvexPoly(vv4, NULL, 0);

    Intersection * ip1=new Intersection(NULL, 1);
    Intersection * ip2=new Intersection(NULL, 1);
    Intersection * ip3=new Intersection(NULL, 1);
    sp=(std::shared_ptr<Shape>) cp1;
    ip1->AddElement(sp);
    sp=(std::shared_ptr<Shape>) cp2;
    ip2->AddElement(sp);
    sp=(std::shared_ptr<Shape>) cp3;
    ip3->AddElement(sp);
    sp=(std::shared_ptr<Shape>) cp4;
    ip1->AddElement(sp);

    Union * up=new Union(&color, 1);

    sp=(std::shared_ptr<Shape>) ip1;
    up->AddElement(sp);
    sp=(std::shared_ptr<Shape>) ip2;
    up->AddElement(sp);
    sp=(std::shared_ptr<Shape>) ip3;
    up->AddElement(sp);
//*/
/*
    std::vector<Vector> vv;
    std::shared_ptr<Shape> sp;

    vv.push_back(Vector(0, StarR));
    vv.push_back(Vector(-StarR, 0));
    vv.push_back(Vector(0, -StarR));
    vv.push_back(Vector(StarR, 0));

    ConvexPoly * cp=new ConvexPoly(vv, NULL, 1);

    Intersection * ip=new Intersection(NULL, 1);

    sp=(std::shared_ptr<Shape>) cp;
    ip->AddElement(sp);

    Union * up=new Union(&color, 1);

    sp=(std::shared_ptr<Shape>) ip;
    up->AddElement(sp);
*/
/*
    std::shared_ptr<Shape> sp;

    Ellipse * ep=new Ellipse(1, 4, 0, 0, 0, -StarR*StarR, NULL, 1);
    
    Intersection * ip=new Intersection(NULL, 1);

    sp=(std::shared_ptr<Shape>) ep;
    ip->AddElement(sp);

    Union * up=new Union(&color, 1);

    sp=(std::shared_ptr<Shape>) ip;
    up->AddElement(sp);
*/
    for (int i=0; i<nnn; i++)
        for (int j=0; j<nnn; j++)
            scene.Add(up->TransformPointer(Rotate(PI/5*2/nnn/nnn*(i*nnn+j)))->TransformPointer(Translate(0.05+i*0.1, 0.05+j*0.1)));

    scene.Draw(bg, abc);

    //image.write_ppm(f, &bg);
    bg.WritePPM(f);
    f.close();
}

void TestStarIntersects(char * outName)
{
    fstream f, fbg;
    f.open(outName, fstream::out | fstream::binary);
    fbg.open("../../pic/50608556_p0.ppm", fstream::in | fstream::binary);
    //fbg.open("../../pic/abc.ppm", fstream::in | fstream::binary);
    if (!fbg.is_open())
        exit(1);
    //PPMImage image(512, InitColor(0.0, 0.0, 0.0, 1.0));
    PPMImage bg(fbg);
    fbg.close();
    Scene scene;

    Color color;
    InitColor(&color, 1, 0, 0, 1);
    std::vector<Vector> vv1, vv2, vv3, vv4;
    std::shared_ptr<Shape> sp;

    vv1.push_back(Vector(sin(0)*StarR, cos(0)*StarR));
    vv1.push_back(Vector(sin(PI*6/5)*StarR, cos(PI*6/5)*StarR));
    vv1.push_back(Vector(sin(PI*4/5)*StarR, cos(PI*4/5)*StarR));
    vv2.push_back(Vector(sin(PI/5)*StarR*sin(PI/10)/cos(PI/5), cos(PI/5)*StarR*sin(PI/10)/cos(PI/5)));
    vv2.push_back(Vector(sin(PI*3/5)*StarR*sin(PI/10)/cos(PI/5), cos(PI*3/5)*StarR*sin(PI/10)/cos(PI/5)));
    vv2.push_back(Vector(sin(PI*2/5)*StarR, cos(PI*2/5)*StarR));
    vv3.push_back(Vector(sin(PI*7/5)*StarR*sin(PI/10)/cos(PI/5), cos(PI*7/5)*StarR*sin(PI/10)/cos(PI/5)));
    vv3.push_back(Vector(sin(PI*9/5)*StarR*sin(PI/10)/cos(PI/5), cos(PI*9/5)*StarR*sin(PI/10)/cos(PI/5)));
    vv3.push_back(Vector(sin(PI*8/5)*StarR, cos(PI*8/5)*StarR));
    vv4.push_back(Vector(sin(PI)*StarR*sin(PI/10)/cos(PI/5), cos(PI)*StarR*sin(PI/10)/cos(PI/5)));
    vv4.push_back(Vector(sin(PI*6/5)*StarR, cos(PI*6/5)*StarR));
    vv4.push_back(Vector(sin(PI*4/5)*StarR, cos(PI*4/5)*StarR));
    
    ConvexPoly * cp1=new ConvexPoly(vv1, NULL, 1);
    ConvexPoly * cp2=new ConvexPoly(vv2, NULL, 1);
    ConvexPoly * cp3=new ConvexPoly(vv3, NULL, 1);
    ConvexPoly * cp4=new ConvexPoly(vv4, NULL, 0);
    
    //cp1->Transformation(Translate(0.5, 0.5));
    //cp2->Transformation(Translate(0.5, 0.5));
    //cp3->Transformation(Translate(0.5, 0.5));
    //cp4->Transformation(Translate(0.5, 0.5));

    Intersection * ip1=new Intersection(NULL, 1);
    Intersection * ip2=new Intersection(NULL, 1);
    Intersection * ip3=new Intersection(NULL, 1);
    sp=(std::shared_ptr<Shape>) cp1;
    ip1->AddElement(sp);
    sp=(std::shared_ptr<Shape>) cp2;
    ip2->AddElement(sp);
    sp=(std::shared_ptr<Shape>) cp3;
    ip3->AddElement(sp);
    sp=(std::shared_ptr<Shape>) cp4;
    ip1->AddElement(sp);

    Union * up=new Union(&color, 1);
/*
    sp=(std::shared_ptr<Shape>) ip1;
    up->AddElement(sp);
    sp=(std::shared_ptr<Shape>) ip2;
    up->AddElement(sp);
    sp=(std::shared_ptr<Shape>) ip3;
    up->AddElement(sp);

    for (int i=0; i<10; i++)
        for (int j=0; j<10; j++)
            scene.Add(up->TransformPointer(Rotate(PI/10*(i+j)))->TransformPointer(Translate(0.05+i*0.1, 0.05+j*0.1)));
*/
    for (int i=0; i<5; i++)
        for (int j=0; j<5; j++)
        {
            //sp=(std::shared_ptr<Shape>) ip1;
            up->AddElement(ip1->TransformPointer(Rotate(PI/25*(i+j)))->TransformPointer(Translate(0.05+1*0.2, 0.05+2*0.2)));
            //sp=(std::shared_ptr<Shape>) ip2;
            up->AddElement(ip2->TransformPointer(Rotate(PI/25*(i+j)))->TransformPointer(Translate(0.05+1*0.2, 0.05+2*0.2)));
            //sp=(std::shared_ptr<Shape>) ip3;
            up->AddElement(ip3->TransformPointer(Rotate(PI/25*(i+j)))->TransformPointer(Translate(0.05+1*0.2, 0.05+2*0.2)));
        }
    scene.Add(up);
    //scene.Add(up->TransformPointer(Translate(0.5, 0.5)));
    scene.Draw(bg, 6);

    //image.write_ppm(f, &bg);
    bg.WritePPM(f);
    f.close();
}

void TestMassive(char * outName)
{
    fstream f, fbg;
    f.open(outName, fstream::out | fstream::binary);
    fbg.open("../../pic/50608556_p0.ppm", fstream::in | fstream::binary);
    //fbg.open("../../pic/abc.ppm", fstream::in | fstream::binary);
    if (!fbg.is_open())
        exit(1);
    //PPMImage image(512, InitColor(0.0, 0.0, 0.0, 1.0));
    PPMImage bg(fbg);
    fbg.close();
    Scene scene;

    Color color;
    InitColor(&color, 1, 0, 0, 1);
    std::vector<Vector> vv;
    std::shared_ptr<Shape> sp;

    vv.push_back(Vector(0.04, 0.04));
    vv.push_back(Vector(0.06, 0.04));
    vv.push_back(Vector(0.06, 0.06));
    vv.push_back(Vector(0.04, 0.06));

    ConvexPoly * cp=new ConvexPoly(vv, NULL, 1);

    Intersection * ip=new Intersection(NULL, 1);

    sp=(std::shared_ptr<Shape>) cp;
    ip->AddElement(sp);

    Union * up=new Union(&color, 1);

    sp=(std::shared_ptr<Shape>) ip;
    up->AddElement(sp);
//    for (int i=0; i<100; i++)
//        up->AddElement(ip->TransformPointer(Translate(0.005*i, 0.005*i)));

    for (int i=0; i<500000; i++)
        scene.Add(up->TransformPointer(Translate(0, 0)));
    //scene.Add(up);

    scene.Draw(bg, 6);

    //image.write_ppm(f, &bg);
    bg.WritePPM(f);
    f.close();
}
void TestPicture(char * outName)
{
    fstream f, fbg;
    f.open(outName, fstream::out | fstream::binary);
    fbg.open("../../pic/50608556_p0.ppm", fstream::in | fstream::binary);
    //fbg.open("../../pic/abc.ppm", fstream::in | fstream::binary);
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
            up->AddElement(sp);//->TransformPointer(Translate(0.5, 0.5))->TransformPointer(Rotate(PI/8)));
        }
        //sp=(std::shared_ptr<Shape>) up;
        scene.Add(up);
    }         

    scene.Draw(bg, 6);

    //image.write_ppm(f, &bg);
    bg.WritePPM(f);
    f.close();

}

int main()
{
    char outName[]="./result/case400";

    int n;
    std::cin>>n;
    std::cout<<"Case: 0 : "<<std::flush;
    TestStarUnions(outName, 10, 6);
/*
    outName[15]=49;
    std::cout<<"Case: 1 : "<<std::flush;
    TestStarUnions(outName, 5, 6);
    outName[15]=50;
    std::cout<<"Case: 2 : "<<std::flush;
    TestStarUnions(outName, 10, 6);
    outName[15]=51;
    std::cout<<"Case: 3 : "<<std::flush;
    TestStarUnions(outName, 50, 6);
    outName[15]=52;
    std::cout<<"Case: 4 : "<<std::flush;
    TestStarUnions(outName, 100, 6);
    outName[15]=53;
    std::cout<<"Case: 5 : "<<std::flush;
    TestStarUnions(outName, 500, 6);
    outName[15]=54;
    std::cout<<"Case: 6 : "<<std::flush;
    TestStarUnions(outName, 1000, 6);
*/
///*
    outName[15]=49;
    std::cout<<"Case: 1 : "<<std::flush;
    TestStarIntersects(outName);
    outName[15]=50;
    std::cout<<"Case: 2 : "<<std::flush;
    TestMassive(outName);
    for (int i=3; i<=n+2; i++)
    {
        outName[15]=48+i%10;
        outName[14]=48+(i/10)%10;
        outName[13]=48+i/100;
        std::cout<<"Case: "<<i<<" : "<<std::flush;
        TestPicture(outName);
    }
//*/
}
