#ifndef WINDOW_H
#define WINDOW_H

#include "renderarea.h"
#include "myshape.h"
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(RenderArea *renderArea);

private slots:
    void shapeChanged();
    void penChanged();
    void colorChanged();
    void undo();
    void redo();

private:
    RenderArea *renderArea;
    QComboBox *shapeComboBox;
    QLabel *shapeLabel;
    QSpinBox *penWidthSpinBox;
    QLabel *penWidthLabel;
    QPushButton *colorButton;
    QLabel *colorLabel;
    QPushButton *backButton;
    QPushButton *forwardButton;
    
};


#endif // WINDOW_H

