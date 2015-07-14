#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <stdio.h>
#include "myshape.h"
#include <QWidget>

using namespace std;

class RenderArea : public QWidget
{
public:
    QPoint startPnt;
    QPoint endPnt;
    bool isPressed;
    int curIndex;
    vector<MyShape*> shapesPtr;
    MyShape* shapePtr = NULL;
    
    RenderArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent* p);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    
public slots:
    void undo();
    void redo();
};

#endif // RENDERAREA_H

