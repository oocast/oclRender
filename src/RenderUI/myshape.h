#ifndef MYSHAPE_H
#define MYSHAPE_H

#include <stdio.h>
#include <QObject>
#include <QPainter>
#include <math.h>
#include <oclrender.h>
#include <QMutex>
#include <interface.h>
#include <QDebug>


using namespace std;

extern QMutex globalMutex;
extern Scene globalScene;
extern vector<QString> classNames;
extern QPointF getTopLeft(QPointF &p1, QPointF &p2);
extern QPointF getBottomRight(QPointF &p1, QPointF &p2);
extern void registerAllClasses();

class MyShape
{
public:
    QString shapeName;
    vector<QPointF> points;
    int lineWidth;
    QColor shapeColor;
    Color color;

    MyShape(){

    }
    
	void setColor(Color *colorPtr){
        int r;
		int g;
		int b;
		int a;
		shapeColor.getRgb(&r,&g,&b,&a);
		InitColor(colorPtr, ((float)r)/256.0, ((float)g)/256.0, ((float)b)/256.0, ((float)a)/256.0);
		ToYUV(colorPtr);
	}

    virtual void draw(QPainter* painter, Scene* scene){

    }

    bool initialized(){
        return (points.size() != 0);
    }

    virtual void update(QPointF p){
        points[1] = p;
    }

    virtual void initialize(QPointF p1, QPointF p2) {
        points.push_back(p1);
        points.push_back(p2);
    }

    virtual bool valid(){
        if(points.size() == 0){
            return false;
        }
        if(points[0] == points[1]){
            return false;
        }
        return true;
    }

};
Q_DECLARE_METATYPE(MyShape)

class FreeBrush : public MyShape
{
public:

    FreeBrush(){
        shapeName = "FreeBrush";
    }

    virtual void draw(QPainter* painter, Scene *scene){
        QPen p;
        p.setWidth(lineWidth);
        p.setColor(shapeColor);
        painter->setPen(p);
        
        setColor(&color);
        
        for(int i=0;i<points.size()-1;i++){
            //painter->drawLine(points[i],points[i+1]);
            
            

			std::shared_ptr<Shape> sp=(std::shared_ptr<Shape>)(new ConvexPoly(LineSegment(Vector(points.at(i).x()/640, points.at(i).y()/640), Vector(points.at(i+1).x()/640, points.at(i+1).y()/640), 0.001*lineWidth, &color,true)));
			scene->Add(sp);
        }
    }

    virtual void update(QPointF p){
        points.push_back(p);
    }

    virtual void initialize(QPointF p1, QPointF p2) {
        points.push_back(p1);
    }

};
Q_DECLARE_METATYPE(FreeBrush)

class StraightLine : public MyShape
{
public:
    StraightLine(){
        shapeName = "StraightLine";
    }

    virtual void draw(QPainter* painter, Scene* scene){
        QPen p;
        p.setWidth(lineWidth);
        p.setColor(shapeColor);
        painter->setPen(p);
        //painter->drawLine(points[0],points[1]);
        
        float d = pow(points.at(0).x()-points.at(1).x(),2)+pow(points.at(0).y()-points.at(1).y(),2);
        //qDebug() << d << endl;
		if(d<=5){
			return;
		}
		
		setColor(&color);
		
		std::shared_ptr<Shape> sp=(std::shared_ptr<Shape>)(new ConvexPoly(LineSegment(Vector(points.at(0).x()/640, points.at(0).y()/640), Vector(points.at(1).x()/640, points.at(1).y()/640), 0.001*lineWidth, &color,true)));
		scene->Add(sp);
		
		
    }
};
Q_DECLARE_METATYPE(StraightLine)

class HollowRectangle : public MyShape
{
public:
    HollowRectangle(){
        shapeName = "HollowRectangle";
    }

    virtual void draw(QPainter* painter, Scene* scene){
        QPen p;
        p.setWidth(lineWidth);
        p.setColor(shapeColor);
        painter->setPen(p);
        QPointF p1 = getTopLeft(points[0],points[1]);
        QPointF p2 = getBottomRight(points[0],points[1]);
        //painter->drawRect(QRect(p1.toPoint(),p2.toPoint()));
        
		setColor(&color);
		
		std::shared_ptr<Shape> sp = Frame(Vector(p1.x()/640, p1.y()/640), Vector(p2.x()/640, p2.y()/640), 0.001*lineWidth, &color);
		scene->Add(sp);

		

        
        
    }
};
Q_DECLARE_METATYPE(HollowRectangle)

class HollowEllipse : public MyShape
{
public:
    HollowEllipse(){
        shapeName = "HollowEllipse";
    }

    virtual void draw(QPainter* painter, Scene* scene){
        QPen p;
        p.setWidth(lineWidth);
        p.setColor(shapeColor);
        painter->setPen(p);
        QPointF p1 = getTopLeft(points[0],points[1]);
        QPointF p2 = getBottomRight(points[0],points[1]);
        //painter->drawEllipse(QRectF(p1,p2));
        
		setColor(&color);
		
		std::shared_ptr<CSG> sp = Ring(Vector(p1.x()/640, p1.y()/640), Vector(p2.x()/640, p2.y()/640), 0.001*lineWidth, &color);
		scene->Add(sp);
        
    }
};
Q_DECLARE_METATYPE(HollowEllipse)

class SolidEllipse : public MyShape
{
public:
    SolidEllipse(){
        shapeName = "SolidEllipse";
    }
    virtual void draw(QPainter* painter, Scene* scene){
        QPointF p1 = getTopLeft(points[0],points[1]);
        QPointF p2 = getBottomRight(points[0],points[1]);
        QPainterPath tmpPath;
        tmpPath.addEllipse(QRectF(p1,p2));
        //painter->fillPath(tmpPath,QBrush(QColor(shapeColor)));
        
		setColor(&color);
		
		std::shared_ptr<Shape> sp(new Ellipse(Vector(p1.x()/640, p1.y()/640), Vector(p2.x()/640, p2.y()/640), &color));
		
		scene->Add(sp);
        

    }
};
Q_DECLARE_METATYPE(SolidEllipse)



class SolidRectangle : public MyShape
{
public:
    SolidRectangle(){
        shapeName = "SolidRectangle";
    }

    virtual void draw(QPainter* painter, Scene* scene){
        QPointF p1 = getTopLeft(points[0],points[1]);
        QPointF p2 = getBottomRight(points[0],points[1]);
        //painter->fillRect(QRect(p1.toPoint(),p2.toPoint()),shapeColor);
        
        
		setColor(&color);
		
		float d1 = pow(points.at(0).x()-points.at(1).x(),2);
		float d2 = pow(points.at(0).y()-points.at(1).y(),2);
		
        //qDebug() << d << endl;
		if(d1<=5 || d2<=5){
			return;
		}
		
		
		std::shared_ptr<Shape> sp=(std::shared_ptr<Shape>)(new ConvexPoly(Rectangle(Vector(points.at(0).x()/640, points.at(0).y()/640), Vector(points.at(1).x()/640, points.at(1).y()/640), &color, true)));
		scene->Add(sp);
        
        
    }

};
Q_DECLARE_METATYPE(SolidRectangle)

class SolidPolygon : public MyShape
{
public:

    bool finished = false;

    SolidPolygon(){
        shapeName = "SolidPolygon";
    }

    bool isTooClose(QPointF &p){
        float dist = pow((points.at(0).x()-p.x()),2)+pow((points.at(0).y()-p.y()),2);
        if(dist < 50){
            return true;
        }
        return false;
    }

    virtual void draw(QPainter* painter, Scene* scene){


		setColor(&color);

        if(!finished){
            QPen p;
            p.setWidth(lineWidth);
            p.setColor(shapeColor);
            painter->setPen(p);
            for(int i=0;i<points.size()-1;i++){
                //painter->drawLine(points[i],points[i+1]);
                
                
				float d = pow(points.at(i).x()-points.at(i+1).x(),2)+pow(points.at(i).y()-points.at(i+1).y(),2);
				//qDebug() << d << endl;
				if(d>5){
					std::shared_ptr<Shape> sp=(std::shared_ptr<Shape>)(new ConvexPoly(LineSegment(Vector(points.at(i).x()/640, points.at(i).y()/640), Vector(points.at(i+1).x()/640, points.at(i+1).y()/640), 0.001*lineWidth, &color,true)));
					scene->Add(sp);
				}
            }
        }else{
            QPolygonF polygon;
            std::vector<Vector> vertices;

            
            for(int i=0;i<points.size();i++){
                polygon << points[i];
                vertices.push_back(Vector(((float)points.at(i).x())/640, ((float)points.at(i).y())/640));
            }
            QPainterPath tmpPath;
            tmpPath.addPolygon(polygon);
            //painter->fillPath(tmpPath,QBrush(QColor(shapeColor)));
            
            
            std::shared_ptr<Shape> sp(new ConvexPoly(vertices, &color,true));
			scene->Add(sp);
            
        }
    }

    void updateModify(QPointF p){
        points[points.size()-1] = p;
    }

    void updateAdd(QPointF p){
        points.push_back(p);
    }

    virtual bool valid(){
        return false;
    }

};
Q_DECLARE_METATYPE(SolidPolygon)

class HollowPolygon : public SolidPolygon
{
public:
    HollowPolygon(){
        shapeName = "HollowPolygon";
    }

    virtual void draw(QPainter* painter, Scene* scene){
        QPen p;
        p.setWidth(lineWidth);
        p.setColor(shapeColor);
        painter->setPen(p);
        
        
		setColor(&color);
        
        
        if(!finished){
            for(int i=0;i<points.size()-1;i++){
                //painter->drawLine(points[i],points[i+1]);
                
                
				float d = pow(points.at(i).x()-points.at(i+1).x(),2)+pow(points.at(i).y()-points.at(i+1).y(),2);
				//qDebug() << d << endl;
				if(d>5){
					std::shared_ptr<Shape> sp=(std::shared_ptr<Shape>)(new ConvexPoly(LineSegment(Vector(points.at(i).x()/640, points.at(i).y()/640), Vector(points.at(i+1).x()/640, points.at(i+1).y()/640), 0.001*lineWidth, &color,true)));
					scene->Add(sp);
				}
            }
        }else{
            QPolygonF polygon;
            for(int i=0;i<points.size();i++){
                polygon << points[i];
            }
            //painter->drawPolygon(polygon);
            
            for(int i=0;i<points.size()-1;i++){
				std::shared_ptr<Shape> sp=(std::shared_ptr<Shape>)(new ConvexPoly(LineSegment(Vector(points.at(i).x()/640, points.at(i).y()/640), Vector(points.at(i+1).x()/640, points.at(i+1).y()/640), 0.001*lineWidth, &color,true)));
				scene->Add(sp);
				if(i==points.size()-2){
					std::shared_ptr<Shape> sp=(std::shared_ptr<Shape>)(new ConvexPoly(LineSegment(Vector(points.at(0).x()/640, points.at(0).y()/640), Vector(points.at(i+1).x()/640, points.at(i+1).y()/640), 0.001*lineWidth, &color,true)));
					scene->Add(sp);
				}
            }
		
            
        }
    }
};
Q_DECLARE_METATYPE(HollowPolygon)

class BezierPath : public MyShape
{
public:
    BezierPath(){
        shapeName = "BezierPath";
    }
    
    virtual void draw(QPainter* painter, Scene* scene){
        
        if(points.size()<=1){
            return;
        }
        QPen p;
        p.setWidth(lineWidth);
        p.setColor(shapeColor);
        painter->setPen(p);
        
        
        setColor(&color);
        
        if(points.size()==2){
            //painter->drawLine(points[0],points[1]);
            
            
            float d = pow(points.at(0).x()-points.at(1).x(),2)+pow(points.at(0).y()-points.at(1).y(),2);

			if(d>5){
				std::shared_ptr<Shape> sp=(std::shared_ptr<Shape>)(new ConvexPoly(LineSegment(Vector(points.at(0).x()/640, points.at(0).y()/640), Vector(points.at(1).x()/640, points.at(1).y()/640), 0.001*lineWidth, &color,true)));
				scene->Add(sp);
			}
			
			
			
            return;
        }
        /*
        QPainterPath path(points[0]);
        path.cubicTo(points[0],points[1],points[2]);
        painter->drawPath(path);
        */
        
        std::shared_ptr<CSG> sp = BezierCurve(Vector(points.at(0).x()/640, points.at(0).y()/640), Vector(points.at(1).x()/640, points.at(1).y()/640),Vector(points.at(2).x()/640, points.at(2).y()/640), 0.001*lineWidth);
        Union *up = new Union(&color,1);
        up->AddElement(sp);
		scene->Add(up);
		
		
    }
    
    virtual bool valid(){
        return false;
    }
    

};
Q_DECLARE_METATYPE(BezierPath)



#endif // MYSHAPE_H
