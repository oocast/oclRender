
#include "renderarea.h"
#include "myshape.h"

#include <QString>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QColor>
#include <QApplication>

MyShape* createShapePtr(QString shapeName){
    int id = QMetaType::type(shapeName.toStdString().c_str());
    MyShape* shapePtr = (MyShape*) QMetaType::create(id);
    return shapePtr;
}

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{    
    curIndex = 0;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMouseTracking(true);
    isPressed = false;
}

void RenderArea::paintEvent(QPaintEvent *p){

    QPainter painter(this);
    
    for(int i=0;i<curIndex;i++){
        MyShape* pShapePtr = shapesPtr[i];
        pShapePtr->draw(&painter);
    }

    if(isPressed){
        shapePtr->draw(&painter);
    }
    
}
    
void RenderArea::mousePressEvent(QMouseEvent *e){
    setCursor(Qt::PointingHandCursor);
    startPnt = e->pos();
    endPnt = e->pos();
    isPressed = true;
}


void RenderArea::mouseMoveEvent(QMouseEvent *e){
    if(isPressed){
        switch (shapePtr->shapeType) {
        case 1:
        {
            endPnt = e->pos();
            myLine* linePtr = new myLine;
            linePtr->startPnt = startPnt;
            linePtr->endPnt = endPnt;
            shapePtr->update(linePtr);
            update();
            startPnt = endPnt;
        }
            break;
        case 2:
        {
            endPnt = e->pos();
            myLine line;
            line.startPnt = startPnt;
            line.endPnt = endPnt;
            shapePtr->update(line);
            update();
        }
            break;
        }
    }
    
}

void RenderArea::mouseReleaseEvent(QMouseEvent *e){
    setCursor(Qt::ArrowCursor);
    isPressed = false;
    
    if(((shapePtr->shapeType==1)&&(shapePtr->linesPtr.size()>0))
            ||((shapePtr->shapeType==2)&&(startPnt != endPnt))) {
        shapesPtr.resize(curIndex);
        shapesPtr.push_back(shapePtr);
        curIndex++;
        QString curName = shapePtr->shapeName;
        int curWidth = shapePtr->lineWidth;
        QColor curColor = shapePtr->shapeColor;
        shapePtr = createShapePtr(curName);
        shapePtr->lineWidth = curWidth;
        shapePtr->shapeColor = curColor;
    }
    
}

void RenderArea::undo()
{
    if(curIndex>0){
        curIndex--;
        update();
    }
}

void RenderArea::redo()
{
    if(curIndex<shapesPtr.size()){
        curIndex++;
        update();
    }
}

