#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <stdio.h>
#include "myshape.h"
#include <QWidget>

using namespace std;

extern MyShape* createShapePtr(QString shapeName);

class RenderArea : public QWidget
{
public:
    QPointF startPnt;
    QPointF endPnt;
    bool isPressed;
    int curIndex;
    vector<MyShape*> shapesPtr;
    MyShape* shapePtr = NULL;

    RenderArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent* p);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void closeEvent(QCloseEvent *e);
    void addShape();
    void forceUpdate();

public slots:
    void undo();
    void redo();
};

#endif // RENDERAREA_H
