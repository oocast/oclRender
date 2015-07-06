#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <vector>
#include <string>
#include <QPen>
#include <QWidget>
#include <QPoint>
#include <QPainter>
#include <QColor>

using namespace std;

enum Shape {Free,Line,Rect,Elli};

typedef struct myLine{
    QPoint startPnt;
    QPoint endPnt;
}myLine;

typedef struct myDrawEvent{
    Shape type;
    vector<myLine*> lines;
    myLine startEnd;
    int width;
    QColor color;
}myDrawEvent;

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);
    
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    void paintEvent(QPaintEvent* p);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    QPoint startPnt;
    QPoint endPnt;
    bool isPressed;
    int curIndex;
    
    vector<myLine*> lines;
    vector<myDrawEvent*> myDrawEvents;

public slots:
    void setShape(Shape shape);
    void setPen(const QPen &pen);
    void undo();
    void redo();

private:
    Shape shape;
    QPen pen;
};


#endif // RENDERAREA_H
