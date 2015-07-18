#include <cmath>

#include "oclrender.h"
#include "pv_helper2.h"

const float PI = 3.14159f;
const float LOCK_FRAME_COUNT = 15.0f;
const float FINAL_RADIUS = 0.02f;
const float QUADANGLE=PI*2/3;
const float HEXRATIO=2;

void CreateSightMarks(Scene & scene, Vector & center)
{
    Color color;
    InitColor(&color, 0.1, 255/256.0f, 0.2, 1);
    ToYUV(&color);
    float thickness=0.005;
    float seglength=1.25;
    float radius=0.042;
    //Ellipse * ep=new Ellipse(1, 1, 0, 0, 0, 0.16);
    //std::shared_ptr<Shape> line=(std::shared_ptr<Shape>) ep;
    //ep->Transformation(Translate(center.x, center.y));
    ConvexPoly * cp=new ConvexPoly(LineSegment(center - Vector(0, radius), center - Vector(0, seglength), thickness));
    std::shared_ptr<Shape> line=(std::shared_ptr<Shape>) cp;

    Transform rot=Rotate(PI*2/3);
    Transform xform1=Around(center, rot);
    Transform xform2=Around(center, rot*rot);
    //Transform xform3=Around(center, rot*rot*rot);
    
    Intersection * ip=new Intersection(NULL, 1);
    ip->AddElement(line);

    Union * up=new Union(&color, 1);
    up->AddElement((std::shared_ptr<Shape>) ip);

    scene.Add(up);
    scene.Add(up->TransformPointer(xform1));
    scene.Add(up->TransformPointer(xform2));
    //scene.Add(up->TransformPointer(xform3));

    return;
}

void CreateTargetMarks(Scene & scene, Vector & center)
{
Color color;
    InitColor(&color, 0.1, 255/256.0f, 0.2, 1);
    ToYUV(&color);
    float thickness=0.005;
    float radius=0.042;
    

    //std::shared_ptr<CSG> up=Ring(radius, radius, center.x, center.y, thickness, &color);
    //scene.Add((std::shared_ptr<Shape>) up);

    std::vector<Vector> vv1, vv2;
/*
    //vv1.push_back(Vector(radius*HEXRATIO*sin(0*QUADANGLE)+center.x, radius*HEXRATIO*cos(0*QUADANGLE)+center.y));
    vv1.push_back(Vector(radius*HEXRATIO*sin(1*QUADANGLE)+center.x, radius*HEXRATIO*cos(1*QUADANGLE)+center.y));
    //vv1.push_back(Vector(radius*HEXRATIO*sin(2*QUADANGLE)+center.x, radius*HEXRATIO*cos(2*QUADANGLE)+center.y));
    vv1.push_back(Vector(radius*HEXRATIO*sin(3*QUADANGLE)+center.x, radius*HEXRATIO*cos(3*QUADANGLE)+center.y));
    //vv1.push_back(Vector(radius*HEXRATIO*sin(4*QUADANGLE)+center.x, radius*HEXRATIO*cos(4*QUADANGLE)+center.y));
    vv1.push_back(Vector(radius*HEXRATIO*sin(5*QUADANGLE)+center.x, radius*HEXRATIO*cos(5*QUADANGLE)+center.y));
    //vv2.push_back(Vector((radius-thickness)*HEXRATIO*sin(0*QUADANGLE)+center.x, (radius-thickness)*HEXRATIO*cos(0*QUADANGLE)+center.y));
    vv2.push_back(Vector((radius-thickness)*HEXRATIO*sin(1*QUADANGLE)+center.x, (radius-thickness)*HEXRATIO*cos(1*QUADANGLE)+center.y));
    //vv2.push_back(Vector((radius-thickness)*HEXRATIO*sin(2*QUADANGLE)+center.x, (radius-thickness)*HEXRATIO*cos(2*QUADANGLE)+center.y));
    vv2.push_back(Vector((radius-thickness)*HEXRATIO*sin(3*QUADANGLE)+center.x, (radius-thickness)*HEXRATIO*cos(3*QUADANGLE)+center.y));
    //vv2.push_back(Vector((radius-thickness)*HEXRATIO*sin(4*QUADANGLE)+center.x, (radius-thickness)*HEXRATIO*cos(4*QUADANGLE)+center.y));
    vv2.push_back(Vector((radius-thickness)*HEXRATIO*sin(5*QUADANGLE)+center.x, (radius-thickness)*HEXRATIO*cos(5*QUADANGLE)+center.y));
*/
    vv1.push_back(Vector(-radius*HEXRATIO*sin(0*QUADANGLE)+center.x, radius*HEXRATIO*cos(0*QUADANGLE)+center.y));
    vv1.push_back(Vector(-radius*HEXRATIO*sin(1*QUADANGLE)+center.x, radius*HEXRATIO*cos(1*QUADANGLE)+center.y));
    vv1.push_back(Vector(-radius*HEXRATIO*sin(2*QUADANGLE)+center.x, radius*HEXRATIO*cos(2*QUADANGLE)+center.y));
    vv2.push_back(Vector(-(radius-thickness)*HEXRATIO*sin(0*QUADANGLE)+center.x, (radius-thickness)*HEXRATIO*cos(0*QUADANGLE)+center.y));
    vv2.push_back(Vector(-(radius-thickness)*HEXRATIO*sin(1*QUADANGLE)+center.x, (radius-thickness)*HEXRATIO*cos(1*QUADANGLE)+center.y));
    vv2.push_back(Vector(-(radius-thickness)*HEXRATIO*sin(2*QUADANGLE)+center.x, (radius-thickness)*HEXRATIO*cos(2*QUADANGLE)+center.y));
    ConvexPoly * cp1=new ConvexPoly(vv1, NULL, 1);
    ConvexPoly * cp2=new ConvexPoly(vv2, NULL, 0);
    Intersection * ip=new Intersection(NULL, 1);
    ip->AddElement((std::shared_ptr<Shape>) cp1);
    ip->AddElement((std::shared_ptr<Shape>) cp2);
    Union * up=new Union(&color, 1);
    up->AddElement((std::shared_ptr<Shape>) ip);
    scene.Add(up);
    return;
}

void CreateScene(Scene & sceneSightMark, Scene & sceneTargetMarks, int i)
{
    CreateSightMarks(sceneSightMark, targetList[i]);
    CreateTargetMarks(sceneTargetMarks, targetList[i]);
    return;
}

void Locking(Scene & sceneSightMark, Scene & sceneTargetMarks, Vector center, int n, int targetNum)
{
    Color color;
    InitColor(&color, 0.1*(LOCK_FRAME_COUNT-n)/LOCK_FRAME_COUNT+255/256.0f*n/LOCK_FRAME_COUNT, 
                           255/256.0f*(LOCK_FRAME_COUNT-n)/LOCK_FRAME_COUNT, 0.2*(LOCK_FRAME_COUNT-n)/LOCK_FRAME_COUNT, 1);
    ToYUV(&color);
    float angle=PI*2/3/LOCK_FRAME_COUNT*n;
    float thickness=0.005;
    float seglength=1.25-0.08*n;
    float radius=0.042-0.002*n;
    ConvexPoly * cp=new ConvexPoly(LineSegment(center - Vector(0, radius), center - Vector(0, seglength), thickness));
    std::shared_ptr<Shape> line=(std::shared_ptr<Shape>) cp;

    Transform rot1=Rotate(angle);
    Transform rot=Rotate(PI*2/3);
    Transform xform0=Around(center, rot1);
    Transform xform1=Around(center, rot*rot1);
    Transform xform2=Around(center, rot*rot*rot1);
    //Transform xform3=Around(center, rot*rot*rot*rot1);
    Intersection * ip1=new Intersection(NULL, 1);
    ip1->AddElement(line);

    Union * up1=new Union(&color, 1);
    up1->AddElement((std::shared_ptr<Shape>) ip1);

    sceneSightMark.nodes[3*targetNum]=up1->TransformPointer(xform0);
    sceneSightMark.nodes[3*targetNum+1]=up1->TransformPointer(xform1);
    sceneSightMark.nodes[3*targetNum+2]=up1->TransformPointer(xform2);
    //sceneSightMark.nodes[4*targetNum+3]=up1->TransformPointer(xform3);


    std::vector<Vector> vv1, vv2;
    vv1.push_back(Vector(-radius*HEXRATIO*sin(0*QUADANGLE+angle)+center.x, radius*HEXRATIO*cos(0*QUADANGLE+angle)+center.y));
    vv1.push_back(Vector(-radius*HEXRATIO*sin(1*QUADANGLE+angle)+center.x, radius*HEXRATIO*cos(1*QUADANGLE+angle)+center.y));
    vv1.push_back(Vector(-radius*HEXRATIO*sin(2*QUADANGLE+angle)+center.x, radius*HEXRATIO*cos(2*QUADANGLE+angle)+center.y));
    vv2.push_back(Vector(-(radius-thickness)*HEXRATIO*sin(0*QUADANGLE+angle)+center.x, (radius-thickness)*HEXRATIO*cos(0*QUADANGLE+angle)+center.y));
    vv2.push_back(Vector(-(radius-thickness)*HEXRATIO*sin(1*QUADANGLE+angle)+center.x, (radius-thickness)*HEXRATIO*cos(1*QUADANGLE+angle)+center.y));
    vv2.push_back(Vector(-(radius-thickness)*HEXRATIO*sin(2*QUADANGLE+angle)+center.x, (radius-thickness)*HEXRATIO*cos(2*QUADANGLE+angle)+center.y));
    ConvexPoly * cp1=new ConvexPoly(vv1, NULL, 1);
    ConvexPoly * cp2=new ConvexPoly(vv2, NULL, 0);
    Intersection * ip2=new Intersection(NULL, 1);
    ip2->AddElement((std::shared_ptr<Shape>) cp1);
    ip2->AddElement((std::shared_ptr<Shape>) cp2);
    Union * up2=new Union(&color, 1);
    up2->AddElement((std::shared_ptr<Shape>) ip2);
    sceneTargetMarks.nodes[targetNum]=(std::shared_ptr<Shape>) up2;
    return;
}

void Locked(Scene & sceneTargetMarks, Vector center, int targetNum)
{
    float angle=PI*4/3/LOCK_FRAME_COUNT;
    sceneTargetMarks.nodes[targetNum]->Transformation(Around(center, Rotate(angle)));

    return;
}

inline void 
AddTarget(float x, float y, int t)
{
    targetList.push_back(Vector(x, y));
    timeList.push_back(t);
}

void 
AddAllTargets(std::ifstream &tl)
{
    while (!tl.eof())
    {
        float x, y;
        int t;
        tl >> x >> y >> t;
        AddTarget(x, y, t);
    }
}

void MainFunc(Scene & sceneSightMark, Scene & sceneTargetMarks, int n)
{
    for (unsigned int i=0; i<targetList.size(); i++)
    {
        if (n==timeList[i]) CreateScene(sceneSightMark, sceneTargetMarks, i);
        else if (n<=timeList[i]+5) continue;
        else if (n<=timeList[i]+LOCK_FRAME_COUNT+5) Locking(sceneSightMark, sceneTargetMarks, targetList[i], n-timeList[i]-5, i);
        else Locked(sceneTargetMarks, targetList[i], i);
    }
    return;
}
