#include "pv_helper.h"

const float PI = 3.14159f;
const float LOCK_FRAME_COUNT = 15.0f;
const float SCALE = 0.8577f;
const float FINAL_RADIUS = 0.03f;

// (circle Int circle) U lineseg U lineseg U lineseg
std::shared_ptr<CSG> 
SightMark(const Vector & center, bool lock, Color * InputColorPointer)
{
    float radius;
    float seglength;
    if (lock)
    {
        seglength = radius = FINAL_RADIUS;
    }
    else
    {
        radius = 0.3;
        seglength = 1.5;
    }

    float thickness = 0.03;
    
    std::shared_ptr<CSG> up(Ring(radius, radius, center.x, center.y, thickness, InputColorPointer));
    std::shared_ptr<Shape> line(new ConvexPoly(LineSegment(center + Vector(0, radius),
        center + Vector(0, seglength), thickness)));
    Transform rot = Rotate(PI / 3.0);
    Transform xform1 = Around(center, rot);
    Transform xform2 = Around(center, rot * rot);
    std::shared_ptr<CSG> ip0(new Intersection(line)),
        ip1(new Intersection(line->TransformPointer(xform1))),
        ip2(new Intersection(line->TransformPointer(xform2)));
    up->AddElement(ip0);
    up->AddElement(ip1);
    up->AddElement(ip2);
    return up;
}

void 
AddAllTargets(std::ifstream &tl)
{
    while (!tl.eof())
    {
        float x, y;
        tl >> x >> y;
        AddTarget(x, y);
    }
}

inline void 
AddTarget(float x, float y)
{
    targetList.push_back(Vector(x, y));
}

void 
CreateTargets(Scene &scene)
{
    Color color;
    InitColor(0.7, 255/256.0f, 0, 1);
    ToYUV(&color);
    for (size_t i = 0; i < targetList.size(); i++)
    {
        scene.Add(new Ellipse(Circle(targetList[i], FINAL_RADIUS - 0.01f, &color)));
    }
}

void 
CreateSightMarks(Scene & scene)
{
    Color color;
    InitColor(&color, 0.1, 255/256.0f, 0.2, 1);
    ToYUV(&color);
    for (size_t i = 0; i < targetList.size(); i++)
    {
        auto sp = SightMark(targetList[i], false, &color);
        scene.Add(sp);
    }
}
// make sure the scene contains all and only sightmarks
// Rotate and Shrink 
void RotateSightMarks(Scene & scene)
{
    Transform rot = Rotate((2.0 / 3.0) * PI / LOCK_FRAME_COUNT);
    Transform sca = Scale(SCALE, SCALE);
    for (size_t i = 0; i < targetList.size(); i++)
    {
        Transform xform = Around(targetList[i], rot * sca);
        auto sp = scene.nodes[i]->TransformPointer(xform);
        scene.Replace(i, sp);
    }
}
// Change all sight marks to lockon mode
void 
LockSightMarks(Scene & scene)
{
    Color color;
    InitColor(&color, 255/256.0f, 0, 0, 1);
    ToYUV(&color);
    for (size_t i = 0; i < targetList.size(); i++)
    {
        auto sp = SightMark(targetList[i], true, &color);
        scene.Replace(i, sp);
    }
}
