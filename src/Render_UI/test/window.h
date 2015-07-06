#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QSpinBox;
QT_END_NAMESPACE
class RenderArea;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:
    void shapeChanged();
    void penChanged();
    void undo();
    void redo();

private:
    RenderArea *renderArea;
    QComboBox *shapeComboBox;
    QLabel *shapeLabel;
    QSpinBox *penWidthSpinBox;
    QLabel *penWidthLabel;
    QComboBox *colorComboBox;
    QLabel *colorLabel;
    QPushButton *backButton;
    QPushButton *forwardButton;
};

#endif // WINDOW_H
