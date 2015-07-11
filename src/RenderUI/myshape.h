#ifndef MYSHAPE_H
#define MYSHAPE_H

#include <QObject>
#include <QPainter>

using namespace std;

struct myLine
{
    QPoint startPnt;
    QPoint endPnt;
};

extern QString shapeNames[];
extern int numShapes;
extern myLine makeShapeLine(myLine line);
extern void registerAllClasses();

class MyShape
{
public:
    QString shapeName;
    int shapeType;
    vector<myLine*> linesPtr;
    myLine line;
    int lineWidth;
    QColor shapeColor;
    
    MyShape(){

    }
    
    virtual void draw(QPainter* painter){
        
    }
    
    void update(myLine* linePtr){
        linesPtr.push_back(linePtr);
    }
    
    void update(myLine line){
        this->line = line;
    }
    
};
Q_DECLARE_METATYPE(MyShape)

class FreeBrush : public MyShape
{
public:
    
    FreeBrush(){
        shapeName = "FreeBrush";
        shapeType = 1;
    }
    
    virtual void draw(QPainter* painter){
        QPen p;
        p.setWidth(lineWidth);
        p.setColor(shapeColor);
        painter->setPen(p);
        for(int i=0;i<linesPtr.size();i++){
            myLine* pLinePtr = linesPtr[i];
            painter->drawLine(pLinePtr->startPnt,pLinePtr->endPnt);
        }
    }
};
Q_DECLARE_METATYPE(FreeBrush)

class StraightLine : public MyShape
{
public:
    StraightLine(){
        shapeName = "StraightLine";
        shapeType = 2;
    }
    
    virtual void draw(QPainter* painter){
        QPen p;
        p.setWidth(lineWidth);
        p.setColor(shapeColor);
        painter->setPen(p);
        painter->drawLine(line.startPnt,line.endPnt);
    }
};
Q_DECLARE_METATYPE(StraightLine)

class HollowRectangle : public MyShape
{
public:
    HollowRectangle(){
        shapeName = "HollowRectangle";
        shapeType = 2;
    }
    
    virtual void draw(QPainter* painter){
        QPen p;
        p.setWidth(lineWidth);
        p.setColor(shapeColor);
        painter->setPen(p);
        myLine pLine = makeShapeLine(line);
        painter->drawRect(QRect(pLine.startPnt,pLine.endPnt));
    }
    
};
Q_DECLARE_METATYPE(HollowRectangle)

class HollowEllipse : public MyShape
{
public:
    HollowEllipse(){
        shapeName = "HollowEllipse";
        shapeType = 2;
    }
    
    virtual void draw(QPainter* painter){
        QPen p;
        p.setWidth(lineWidth);
        p.setColor(shapeColor);
        painter->setPen(p);
        myLine pLine = makeShapeLine(line);
        painter->drawEllipse(QRectF(pLine.startPnt,pLine.endPnt));
    }
    
};
Q_DECLARE_METATYPE(HollowEllipse)

class SolidRectangle : public MyShape
{
public:
    SolidRectangle(){
        shapeName = "SolidRectangle";
        shapeType = 2;
    }
    
    virtual void draw(QPainter* painter){
        myLine pLine = makeShapeLine(line);
        painter->fillRect(QRect(pLine.startPnt,pLine.endPnt),shapeColor);
    }
    
};
Q_DECLARE_METATYPE(SolidRectangle)


#endif // MYSHAPE_H

