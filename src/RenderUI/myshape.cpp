
#include "myshape.h"

using namespace std;

Scene globalScene;
QMutex globalMutex;

vector<QString> classNames = {"StraightLine","BezierPath","SolidRectangle","HollowRectangle","SolidEllipse","HollowEllipse","SolidPolygon","HollowPolygon","FreeBrush"};

QPointF getTopLeft(QPointF &p1, QPointF &p2){
    float x_min = min(p1.x(),p2.x());
    float y_max = max(p1.y(),p2.y());
    return QPointF(x_min,y_max);
}

QPointF getBottomRight(QPointF &p1, QPointF &p2){
    float x_max = max(p1.x(),p2.x());
    float y_min = min(p1.y(),p2.y());
    return QPointF(x_max,y_min);
}

extern void registerAllClasses(){
    qRegisterMetaType<MyShape>();
    qRegisterMetaType<FreeBrush>();
    qRegisterMetaType<StraightLine>();
    qRegisterMetaType<HollowRectangle>();
    qRegisterMetaType<HollowEllipse>();
    qRegisterMetaType<SolidRectangle>();
    qRegisterMetaType<SolidEllipse>();
    qRegisterMetaType<SolidPolygon>();
    qRegisterMetaType<HollowPolygon>();
    qRegisterMetaType<BezierPath>();
}
