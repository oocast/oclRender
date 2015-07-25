#include <cmath>
#include <iostream>
#include "interface.h"

std::shared_ptr<Shape> 
Frame(const Vector &vertex1, const Vector &vertex2, float thickness,
      const Color * inputColorPointer)
{
    Vector low = vertex1.Min(vertex2);
    Vector high = vertex1.Max(vertex2);
    Vector innerWay(thickness, thickness);
    low = low + innerWay;
    high = high - innerWay;

    std::shared_ptr<CSG> u(new Union(inputColorPointer, true));
    
    
    if(low.x >= high.x || low.y >= high.y){
    	return u;
    }else{
        std::shared_ptr<CSG> in(new Intersection(inputColorPointer, true));
		std::shared_ptr<Shape> outer(new ConvexPoly(Rectangle(vertex1, vertex2, inputColorPointer)));
		std::shared_ptr<Shape> inner(new ConvexPoly(Rectangle(low, high, inputColorPointer, false)));
		in->AddElement(outer);
		in->AddElement(inner);
		u->AddElement(in);
    }
    return u;
}

std::shared_ptr<CSG> 
Ring(float halfWidth, float halfHeight, float centerX, 
     float centerY, float thickness,
     const Color * inputColorPointer)
{
    Vector center(centerX, centerY);
    Vector halfSize(halfWidth, halfHeight);
    Vector low = center - halfSize;
    Vector high = center + halfSize;

    std::shared_ptr<CSG> u(new Union(inputColorPointer, true));
    std::shared_ptr<CSG> in(new Intersection(inputColorPointer, true));
    std::shared_ptr<Shape> outer(new Ellipse(low, high, inputColorPointer));

    Vector innerWay(thickness, thickness);
    low = low + innerWay;
    high = high - innerWay;
    std::shared_ptr<Shape> inner(new Ellipse(low, high, inputColorPointer, false));
    in->AddElement(outer);
    in->AddElement(inner);
    u->AddElement(in);
    return (std::shared_ptr<CSG>) u;
}

std::shared_ptr<CSG> 
Ring(const Vector &boundVertex1, const Vector &boundVertex2, float thickness,
     const Color * inputColorPointer)
{
    Vector center = (boundVertex1 + boundVertex2) * 0.5F;
    Vector halfSize = boundVertex1.Max(boundVertex2) - center;
    Vector low = center - halfSize;
    Vector high = center + halfSize;

    std::shared_ptr<CSG> u(new Union(inputColorPointer, true));
    
	if(low.x >= high.x || low.y >= high.y){
    	return u;
    }
    
    std::shared_ptr<CSG> in(new Intersection(inputColorPointer, true));
    std::shared_ptr<Shape> outer(new Ellipse(low, high, inputColorPointer));

    Vector innerWay(thickness, thickness);
    low = low + innerWay;
    high = high - innerWay;
    
	if(low.x >= high.x || low.y >= high.y){
    	return u;
    }
    
    std::shared_ptr<Shape> inner(new Ellipse(low, high, inputColorPointer, false));
    in->AddElement(outer);
    in->AddElement(inner);
    u->AddElement(in);
    return (std::shared_ptr<CSG>) u;
}

std::shared_ptr<Shape> 
Arrow(const Vector &boundVertex1, const Vector &boundVertex2,
      int direction, const Color * inputColorPointer,
      float lengthRatio, float widthRatio)
{
    Vector low = boundVertex1.Min(boundVertex2);
    Vector high = boundVertex1.Max(boundVertex2);
    Vector size = high - low;
    Vector handleVertex1, handleVertex2, triangleVertex1, triangleVertex2, triangleVertex3;
    float xin = 0.5F * (1.0F - widthRatio) * size.x;
    float yin = 0.5F * (1.0F - widthRatio) * size.y;

    // direction: 0right, 1up, 2left, 3down
    switch (direction)
    {
    case 0:
        handleVertex1 = low + Vector(0.0F, yin);
        handleVertex2 = Vector(low.x + lengthRatio * size.x, high.y - yin);
        triangleVertex1 = high - Vector(0.0F, 0.5F * size.y);
        triangleVertex2 = Vector(handleVertex2.x, high.y);
        triangleVertex3 = Vector(handleVertex2.x, low.y);
        break;
    case 1:
        handleVertex1 = low + Vector(xin);
        handleVertex2 = Vector(high.x - xin, low.y + lengthRatio * size.y);
        triangleVertex1 = high - Vector(0.5 * size.x);
        triangleVertex2 = Vector(low.x, handleVertex2.y);
        triangleVertex3 = Vector(high.x, handleVertex2.y);
        break;
    case 2:
        handleVertex1 = Vector(high.x - lengthRatio * size.x, low.y + yin);
        handleVertex2 = high - Vector(0.0F, yin);
        triangleVertex1 = low + Vector(0.0F, 0.5 * size.y);
        triangleVertex2 = Vector(handleVertex1.x, low.y);
        triangleVertex3 = Vector(handleVertex1.x, high.y);
        break;
    case 3:
        handleVertex1 = Vector(low.x + xin, high.y - lengthRatio * size.y);
        handleVertex2 = high - Vector(xin);
        triangleVertex1 = low + Vector(0.5 * size.x);
        triangleVertex2 = Vector(high.x, handleVertex1.y);
        triangleVertex3 = Vector(low.x, handleVertex1.y);
        break;
    }

    std::shared_ptr<CSG> u(new Union(inputColorPointer, true));
    std::shared_ptr<CSG> in1(new Intersection(inputColorPointer, true));
    std::shared_ptr<CSG> in2(new Intersection(inputColorPointer, true));
    std::shared_ptr<Shape> rect(new ConvexPoly(Rectangle(handleVertex1, handleVertex2)));
    std::shared_ptr<Shape> tri(new ConvexPoly(Triangle({triangleVertex1, triangleVertex2, triangleVertex3})));
    in1->AddElement(rect);
    in2->AddElement(tri);
    u->AddElement(in1);
    u->AddElement(in2);
    return u;
}

std::shared_ptr<CSG> BrushInit(const Vector &vertex1, const Vector &vertex2, float thickness,
                               Vector &endVertex, const Color * inputColorPointer)
{
    std::shared_ptr<Shape> seg(new ConvexPoly(LineSegment(vertex1, vertex2, thickness)));
    std::shared_ptr<CSG> u(new Union(inputColorPointer, true));
    std::shared_ptr<CSG> in(new Intersection(nullptr, true));
    endVertex = vertex2;
    in->AddElement(seg);
    u->AddElement(in);
    return u;
}

std::shared_ptr<CSG> BrushAppend(const Vector &vertex, std::shared_ptr<CSG> prevBrush,
                                 float thickness, Vector &endVertex)
{
    std::shared_ptr<CSG> in(new Intersection(nullptr, true));
    std::shared_ptr<Shape> seg(new ConvexPoly(LineSegment(endVertex, vertex, thickness)));
    endVertex = vertex;
    in->AddElement(seg);
    prevBrush->AddElement(in);
    return prevBrush;
}

std::shared_ptr<CSG> BezierCurve(const Vector & v1, const Vector & v2, const Vector & v3, const float delta)
{
    std::vector<Vector> vv;

    vv.push_back(v1);
    vv.push_back(v2);
    vv.push_back(v3);

    return BezierCurve(vv, delta);
}

std::shared_ptr<CSG> BezierCurve(const std::vector<Vector> & vv, const float delta)
{
    const Vector & v1=vv[0];
    const Vector & v2=vv[1];
    const Vector & v3=vv[2];
    float x0, x1, x2, y0, y1, y2, z0, z1, z2, a, b, c, d, e, f;
    x0=v1.x;
    x1=v2.x-v1.x;
    x2=v3.x-2*v2.x+v1.x;
    y0=v1.y;
    y1=v2.y-v1.y;
    y2=v3.y-2*v2.y+v1.y;
    z0=x1*y2-x2*y1;
    z1=x0*y2-x2*y0;
    z2=x0*y1-x1*y0;
    a=y2*y2;
    b=-x2*y2;
    c=x2*x2;
    d=-y2*z1+2*y1*z0;
    e=x2*z1-2*x1*z0;
    f=z1*z1-4*z2*z0;

    std::shared_ptr<CSG> res=ParabolaRing(a, c, 2*b, 2*d, 2*e, f, delta);
    
    std::shared_ptr<Shape> sp;

    ConvexPoly * cp=new ConvexPoly(vv, NULL, 1);

    sp=(std::shared_ptr<Shape>) cp;

    res->AddElement(sp);
    return res;
}

std::shared_ptr<CSG> EllipseRing(const float a, const float b, const float c, const float d, 
                                 const float e, const float f, const float delta)
{
    std::shared_ptr<Shape> sp;
    std::shared_ptr<CSG> res;

    Ellipse * ep1=new Ellipse(a, b, c, d, e, f, NULL, 1);
    
    Intersection * ip=new Intersection(NULL, 1);

    sp=(std::shared_ptr<Shape>) ep1;
    ip->AddElement(sp);

    Vector center=ep1->AccessCenter();
    float xc=center.x;
    float yc=center.y;
    float ft=a*xc*xc+b*yc*yc+c*xc*yc-f;
    float invSquareSemiMajAxis=(a+b-sqrt((a-b)*(a-b)+c*c))/2/ft;
    float invSquareSemiMinAxis=(a+b+sqrt((a-b)*(a-b)+c*c))/2/ft;
    float squareCos=-ft*(a*invSquareSemiMajAxis-b*invSquareSemiMinAxis)/(a+b)/sqrt((a-b)*(a-b)+c*c);
    float squareSin=1-squareCos;
    float squareSemiMajAxis1=1/invSquareSemiMajAxis+delta*delta-2*delta*sqrt(1/invSquareSemiMajAxis);
    float squareSemiMinAxis1=1/invSquareSemiMinAxis+delta*delta-2*delta*sqrt(1/invSquareSemiMinAxis);
    float A=squareSemiMajAxis1*squareSin+squareSemiMinAxis1*squareCos;
    float B=squareSemiMajAxis1*squareCos+squareSemiMinAxis1*squareSin;
    float C=2*(squareSemiMinAxis1-squareSemiMajAxis1)*copysign(sqrt(squareSin*squareCos), -c);
    float D=-2*A*xc-C*yc;
    float E=-2*B*yc-C*xc;
    float F=A*xc*xc+B*yc*yc+C*xc*yc-squareSemiMajAxis1*squareSemiMinAxis1;

    Ellipse * ep2=new Ellipse(A, B, C, D, E, F, NULL, 0);

    sp=(std::shared_ptr<Shape>) ep2;
    ip->AddElement(sp);    

    res=(std::shared_ptr<CSG>) ip;
    return res;
}

std::shared_ptr<CSG> ParabolaRing(const float a, const float b, const float c, const float d, 
                                 const float e, const float f, const float delta)
{
    std::shared_ptr<Shape> sp;
    std::shared_ptr<CSG> res;

    Intersection * ip=new Intersection(NULL, 1);
    res=(std::shared_ptr<CSG>) ip;
    if (a+b==0) return res;

    Ellipse * ep1=new Ellipse(a, b, c, d, e, f, NULL, 1);
    
    sp=(std::shared_ptr<Shape>) ep1;
    ip->AddElement(sp);

    float sinTheta=sqrt(b/(a+b));
    float cosTheta=copysign(sqrt(a/(a+b)), c);
    //float p=(d*sinTheta-e*cosTheta)/2/(a+b);
    float P=(d*sinTheta-e*cosTheta);
    //float alpha=(-d*cosTheta-e*sinTheta)/2/(a+b);
    //float beta=(alpha*alpha-f/(a+b))/2/p;
    //float xc=alpha*cosTheta+beta*sinTheta;
    //float yc=alpha*sinTheta-beta*cosTheta;
    //float F=f+2*delta*fabs(p)*(a+b);
    float F=f+delta*fabs(P);

    Ellipse * ep2=new Ellipse(a, b, c, d, e, F, NULL, 0);

    sp=(std::shared_ptr<Shape>) ep2;
    ip->AddElement(sp);    

    return res;
}
