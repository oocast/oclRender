#include "myshape.h"

using namespace std;

QString shapeNames[5] = {"FreeBrush","StraightLine","HollowRectangle","HollowEllipse","SolidRectangle"};
int numShapes = 5;

myLine makeShapeLine(myLine line){
    int x_min = min(line.startPnt.x(),line.endPnt.x());
    int x_max = max(line.startPnt.x(),line.endPnt.x());
    int y_min = min(line.startPnt.y(),line.endPnt.y());
    int y_max = max(line.startPnt.y(),line.endPnt.y());
    QPoint p1(x_min,y_max);
    QPoint p2(x_max,y_min);
    myLine result;
    result.startPnt = p1;
    result.endPnt = p2;
    return result;
}

extern void registerAllClasses(){
    qRegisterMetaType<MyShape>();
    qRegisterMetaType<FreeBrush>();
    qRegisterMetaType<StraightLine>();
    qRegisterMetaType<HollowRectangle>();
    qRegisterMetaType<HollowEllipse>();
    qRegisterMetaType<SolidRectangle>();
}
