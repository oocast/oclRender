#include "renderarea.h"
#include <stdio.h>
#include <QTextStream>
#include "renderarea.h"
#include <QString>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <string.h>
#include <string>
#include <QColor>


using namespace std;
QTextStream cout(stdout,  QIODevice::WriteOnly);


RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{    
    curIndex = 0;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    this->setMouseTracking(true);
    this->isPressed = false;
    
}

void RenderArea::paintEvent(QPaintEvent *p){
    
    QPainter painter(this);
    
    for(int i=0;i<curIndex;i++){
        myDrawEvent* pEvent = myDrawEvents[i];
        QPen pPen;
        pPen.setWidth(pEvent->width);
        pPen.setColor(pEvent->color);
        painter.setPen(pPen);
        
        switch (pEvent->type) {
        case Free:
        {
            for(int j=0;j<pEvent->lines.size();j++){
                myLine* pLine = pEvent->lines[j];
                painter.drawLine(pLine->startPnt,pLine->endPnt);
            }
        }
            break;
        case Line:
        {
            painter.drawLine(pEvent->startEnd.startPnt,pEvent->startEnd.endPnt);
        }
            break;
        
        case Elli:
        {
            QPoint p1;
            p1.setX(min(pEvent->startEnd.startPnt.x(),pEvent->startEnd.endPnt.x()));
            p1.setY(max(pEvent->startEnd.startPnt.y(),pEvent->startEnd.endPnt.y()));
            QPoint p2;
            p2.setX(max(pEvent->startEnd.startPnt.x(),pEvent->startEnd.endPnt.x()));
            p2.setY(min(pEvent->startEnd.startPnt.y(),pEvent->startEnd.endPnt.y())); 
            painter.drawEllipse(QRectF(p1,p2));
            
        }
            break;
        case Rect:
        {
            QPoint p1;
            p1.setX(min(pEvent->startEnd.startPnt.x(),pEvent->startEnd.endPnt.x()));
            p1.setY(max(pEvent->startEnd.startPnt.y(),pEvent->startEnd.endPnt.y()));
            QPoint p2;
            p2.setX(max(pEvent->startEnd.startPnt.x(),pEvent->startEnd.endPnt.x()));
            p2.setY(min(pEvent->startEnd.startPnt.y(),pEvent->startEnd.endPnt.y())); 
            
            painter.drawRect(QRect(p1,p2));
        }
            break;
        }
    }

    if(isPressed){
        painter.setPen(pen);
        switch (shape) {
        case Free:
        {
            for(int i=0;i<lines.size();i++){
                myLine* pLine = lines[i];
                painter.drawLine(pLine->startPnt,pLine->endPnt);
            }
        }
            break;
        case Line:
        {
            if(startPnt!=endPnt){
                painter.drawLine(startPnt,endPnt);
            }
        }
            break;
            
        case Elli:
        {
            if(startPnt!=endPnt){
                QPoint p1;
                p1.setX(min(startPnt.x(),endPnt.x()));
                p1.setY(max(startPnt.y(),endPnt.y()));
                QPoint p2;
                p2.setX(max(startPnt.x(),endPnt.x()));
                p2.setY(min(startPnt.y(),endPnt.y())); 
                painter.drawEllipse(QRectF(p1,p2));
            }
        }
            break;
        case Rect:
        {
            if(startPnt!=endPnt){
                QPoint p1;
                p1.setX(min(startPnt.x(),endPnt.x()));
                p1.setY(max(startPnt.y(),endPnt.y()));
                QPoint p2;
                p2.setX(max(startPnt.x(),endPnt.x()));
                p2.setY(min(startPnt.y(),endPnt.y())); 
                painter.drawRect(QRect(p1,p2));
            }
        }
            break;
        }
    }
    
}
    
void RenderArea::mousePressEvent(QMouseEvent *e){
    setCursor(Qt::PointingHandCursor);
    startPnt = e->pos();
    endPnt = e->pos();
    this->isPressed = true;
    switch (shape) {
    case Free:
    {
        this->lines.clear();
    }
        break;
    case Line:
    {
    }
        break;
    case Elli:
    {
    }
        break;
    case Rect:
    {
    }
        break;
    }

}


void RenderArea::mouseMoveEvent(QMouseEvent *e){
    if(this->isPressed){
        switch (shape) {
        case Free:
        {
            endPnt = e->pos();
            myLine* line = new myLine;
            line->startPnt = startPnt;
            line->endPnt = endPnt;
            this->lines.push_back(line);
            update();
            startPnt = endPnt;
        }
            break;
        case Line:
        {
            endPnt = e->pos();
            update();
        }
            break;
        
        case Elli:
        {
            endPnt = e->pos();
            update();
        }
            break;
        case Rect:
        {
            endPnt = e->pos();
            update();
        }
            break;
        
        }
    }
}

void RenderArea::mouseReleaseEvent(QMouseEvent *e){
    setCursor(Qt::ArrowCursor);
    this->isPressed = false;
    myDrawEvent* pEvent = new myDrawEvent;
    pEvent->width = pen.width();
    pEvent->color = pen.color();
    switch (shape) {
    case Free:
    {
        if(lines.size()>0){
            pEvent->lines = lines;
            pEvent->type = Free;
            lines.clear();
            if(curIndex<myDrawEvents.size()){
                myDrawEvents.resize(curIndex);
            }            myDrawEvents.push_back(pEvent);
            curIndex++;
        }
    }
        break;
    case Line:
    {
        if(startPnt != endPnt){
            myLine pL;
            pL.startPnt = startPnt;
            pL.endPnt = endPnt;
            pEvent->startEnd = pL;
            pEvent->type = Line;
            if(curIndex<myDrawEvents.size()){
                myDrawEvents.resize(curIndex);
            }            myDrawEvents.push_back(pEvent);
            curIndex++;
        }
    }
        break;
        
    case Elli:
    {
        if(startPnt != endPnt){
            myLine pL;
            pL.startPnt = startPnt;
            pL.endPnt = endPnt;
            pEvent->startEnd = pL;
            pEvent->type = Elli;
            if(curIndex<myDrawEvents.size()){
                myDrawEvents.resize(curIndex);
            }            myDrawEvents.push_back(pEvent);
            curIndex++;
        }
    }
        break;
    case Rect:
    {
        if(startPnt != endPnt){
            myLine pL;
            pL.startPnt = startPnt;
            pL.endPnt = endPnt;
            pEvent->startEnd = pL;
            pEvent->type = Rect;
            if(curIndex<myDrawEvents.size()){
                myDrawEvents.resize(curIndex);
            }
            myDrawEvents.push_back(pEvent);
            curIndex++;
        }
    }
        break;
        
    }
    cout << "-----------------------------------------" << endl;
    cout << QString::number(curIndex) << endl;
    cout << QString::number(myDrawEvents.size()) << endl;
    
}



QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}


QSize RenderArea::sizeHint() const
{
    return QSize(400, 300);
}

void RenderArea::setShape(Shape shape)
{
    this->shape = shape;
}


void RenderArea::setPen(const QPen &pen)
{
    this->pen = pen;
}

void RenderArea::undo()
{
    if(curIndex>0){
        curIndex--;
        update();
        cout << "-----------------------------------------" << endl;
        cout << QString::number(curIndex) << endl;
        cout << QString::number(myDrawEvents.size()) << endl;
    }
}

void RenderArea::redo()
{
    
    if(curIndex<myDrawEvents.size()){
        curIndex++;
        update();
        cout << "-----------------------------------------" << endl;
        cout << QString::number(curIndex) << endl;
        cout << QString::number(myDrawEvents.size()) << endl;
    }
}

