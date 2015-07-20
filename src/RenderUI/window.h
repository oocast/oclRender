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
    void closeEvent(QCloseEvent *e);


private slots:
    void shapeChanged();
    void penChanged();
    void colorChanged();
    void undo();
    void redo();

private:
    RenderArea *renderArea;
    QButtonGroup *shapeButtonGroup;
    QSpinBox *penWidthSpinBox;
    QPushButton *colorButton;
    QPushButton *backButton;
    QPushButton *forwardButton;
    

};


#endif // WINDOW_H
