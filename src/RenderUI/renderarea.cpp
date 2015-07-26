#include "renderarea.h"
#include "myshape.h"

#include <QString>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QColor>
#include <QApplication>
#include <QDebug>
#include <QThread>


#include <getopt.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <ctype.h>
#include <va/va.h>
#include <va/va_drmcommon.h>
#include "va_display.h"
#include "oclrender.h"
#include "interface.h"
#include "pv_helper.h"


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

void RenderArea::closeEvent(QCloseEvent *e){
    //qDebug()<<"close renderArea"<< endl;
}

class SettingThread : public QThread{

public:
	Scene scene;

	SettingThread(Scene &scene){
		this->scene = scene;
	}
	
	virtual void run(){
		globalMutex.lock();
		globalScene = scene;
		globalMutex.unlock();
	}

};


void RenderArea::paintEvent(QPaintEvent *p){

	Scene scene;

    QPainter painter(this);
    for(int i=0;i<curIndex;i++){
        MyShape* pShapePtr = shapesPtr[i];
        pShapePtr->draw(&painter,&scene);
    }
    if(shapePtr->initialized()){
        shapePtr->draw(&painter,&scene);
    }

	SettingThread *t = new SettingThread(scene);
	t->start();
}

void RenderArea::mousePressEvent(QMouseEvent *e){


    setCursor(Qt::PointingHandCursor);
    startPnt = e->pos();
    endPnt = e->pos();
    
	if(e->button() == Qt::RightButton){
    	if(shapePtr->shapeName == "HollowPolygon" || shapePtr->shapeName == "SolidPolygon"){
    		SolidPolygon* pShapePtr = dynamic_cast<SolidPolygon*>(shapePtr);
    		pShapePtr->points.clear();
    		update();
    	}
        return;
    }
    
    isPressed = true;
        if(shapePtr->shapeName == "BezierPath"){
        if(shapePtr->points.size()<=2){
            shapePtr->points.push_back(startPnt);
        }else{
            addShape();
        }
        return;
    }
    if(shapePtr->shapeName == "HollowPolygon" || shapePtr->shapeName == "SolidPolygon"){
        SolidPolygon* pShapePtr = dynamic_cast<SolidPolygon*>(shapePtr);
        if(!pShapePtr->initialized()){
            pShapePtr->initialize(startPnt,endPnt);
        }else{
            if(pShapePtr->isTooClose(endPnt)){
                if(pShapePtr->points.size()<=3){
                    shapePtr->points.clear();
                    update();
                    return;
                }
                pShapePtr->points.pop_back();
                pShapePtr->finished = true;
                addShape();
            }else{
                pShapePtr->updateAdd(endPnt);
            }
        }
        update();
        return;
    }
}


void RenderArea::mouseMoveEvent(QMouseEvent *e){
    if(shapePtr->shapeName == "BezierPath"){
        if(!shapePtr->initialized()){
            return;
        }
        endPnt = e->pos();
        if(shapePtr->points.size()==1){
            shapePtr->points.push_back(endPnt);
        }else if(shapePtr->points.size()==2){
            shapePtr->points.pop_back();
            shapePtr->points.push_back(endPnt);
        }else{
            shapePtr->points.at(1)=endPnt;
        }
        update();
        return;
    }
    if(shapePtr->shapeName == "SolidPolygon" || shapePtr->shapeName == "HollowPolygon"){
        SolidPolygon* pShapePtr = dynamic_cast<SolidPolygon*>(shapePtr);
        if(!pShapePtr->initialized()){
            return;
        }
        endPnt = e->pos();
        if(pShapePtr->isTooClose(endPnt)){
            pShapePtr->updateModify(pShapePtr->points.at(0));
        }else{
            pShapePtr->updateModify(endPnt);
        }
        update();
        return;
    }
    if(isPressed){
        endPnt = e->pos();
        if(!shapePtr->initialized()){
            shapePtr->initialize(startPnt,endPnt);
        }
        shapePtr->update(endPnt);
        update();
    }
}

void RenderArea::mouseReleaseEvent(QMouseEvent *e){

    if(e->button() == Qt::RightButton){
        return;
    }
    setCursor(Qt::ArrowCursor);
    isPressed = false;
    if(shapePtr->valid()) {
        addShape();
        update();
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

void RenderArea::addShape(){
    for(int i=curIndex;i<shapesPtr.size();i++){
        delete shapesPtr[i];
        shapesPtr[i]=NULL;
    }
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
