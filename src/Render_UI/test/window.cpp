#include "renderarea.h"
#include "window.h"

#include <QtWidgets>
#include <QColor>

const int IdRole = Qt::UserRole;

QMutex mutex;

Window::Window()
{
    int x, y;
    int screenWidth;
    int screenHeight;
    int WIDTH = 400;
    int HEIGHT = 400;
    QDesktopWidget *desktop = QApplication::desktop();
    screenWidth = desktop->width();
    screenHeight = desktop->height();
    x = (screenWidth - WIDTH) / 2;
    y = (screenHeight - HEIGHT) / 2;
    setGeometry(x, y, WIDTH, HEIGHT);
    
/*
    QProcess *proc;
    proc = new QProcess(this);
    proc->execute("open /users/yaoxiao/Desktop/test.docx");
    QThread::sleep(1);
    proc->execute("osascript /users/yaoxiao/Desktop/setsize.scpt");
*/
    
    //setAttribute(Qt::WA_TranslucentBackground, true);
    
    
    renderArea = new RenderArea;
    
    
    //renderArea->setAttribute(Qt::WA_TranslucentBackground, true);
    
    shapeComboBox = new QComboBox;
    
    shapeComboBox->addItem(tr("Free Draw"), Free);
    shapeComboBox->addItem(tr("Line"), Line);
    shapeComboBox->addItem(tr("Rectangle"), Rect);
    shapeComboBox->addItem(tr("Ellipse"), Elli);
    shapeLabel = new QLabel(tr("&Shape:"));
    shapeLabel->setBuddy(shapeComboBox);


    penWidthSpinBox = new QSpinBox;
    penWidthSpinBox->setRange(1, 20);
    penWidthLabel = new QLabel(tr("Pen &Width:"));
    penWidthLabel->setBuddy(penWidthSpinBox);

    
    colorComboBox = new QComboBox;
    colorComboBox->addItem(tr("Black"),QColor("black"));
    colorComboBox->addItem(tr("Blue"),QColor("blue"));
    colorComboBox->addItem(tr("Red"),QColor("red"));
    colorLabel = new QLabel(tr("&Color:"));
    colorLabel->setBuddy(colorComboBox);
    
    backButton = new QPushButton;
    backButton->setText("Undo");
    
    forwardButton = new QPushButton;
    forwardButton->setText("Redo");

    connect(shapeComboBox, SIGNAL(activated(int)),
            this, SLOT(shapeChanged()));
    connect(penWidthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(penChanged()));
    
    connect(colorComboBox, SIGNAL(activated(int)),
            this, SLOT(penChanged()));
    connect(backButton, SIGNAL(clicked()),
            this, SLOT(undo()));
    connect(forwardButton, SIGNAL(clicked()),
            this, SLOT(redo()));

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    mainLayout->addWidget(shapeLabel, 2, 0, Qt::AlignRight);
    mainLayout->addWidget(shapeComboBox, 2, 1);
    mainLayout->addWidget(penWidthLabel, 3, 0, Qt::AlignRight);
    mainLayout->addWidget(penWidthSpinBox, 3, 1);    
    mainLayout->addWidget(colorLabel, 4, 0, Qt::AlignRight);
    mainLayout->addWidget(colorComboBox, 4, 1);
    mainLayout->addWidget(backButton,2,2);
    mainLayout->addWidget(forwardButton,3,2);
    
    
    setLayout(mainLayout);

    shapeChanged();
    penChanged();
    setWindowTitle(tr("render test"));
    
    //proc->execute("open -a \"/Users/yaoxiao/build-test-Desktop_Qt_5_4_2_clang_64bit-Debug/test.app/Contents/MacOS/test\"");
    

}

void Window::shapeChanged()
{
    Shape shape = Shape(shapeComboBox->itemData(
            shapeComboBox->currentIndex(), IdRole).toInt());
    renderArea->setShape(shape);
    
}

void Window::penChanged()
{
    int width = penWidthSpinBox->value();

    QColor color = QColor(colorComboBox->itemData(
                          colorComboBox->currentIndex()).value<QColor>());
    
    renderArea->setPen(QPen(color, width));
}

void Window::undo()
{
    renderArea->undo();
}

void Window::redo()
{
    renderArea->redo();
}







