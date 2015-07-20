#include "window.h"
#include "myshape.h"
#include "renderarea.h"

#include <stdio.h>
#include <QtWidgets>
#include <QColor>
#include <QDebug>

const int IdRole = Qt::UserRole;
QColor defaultColor = Qt::red;

using namespace std;

void setColor(QPushButton* button, QColor color){
    QPalette pal = button->palette();
    pal.setColor(QPalette::Button, color);
    button->setAutoFillBackground(true);
    button->setFlat(true);
    button->setPalette(pal);
    button->update();
}

Window::Window(RenderArea *ra)
{
    renderArea = ra;

    shapeComboBox = new QComboBox;

    for(int i=0;i<classNames.size();i++){
        QString pName = classNames[i];
        shapeComboBox->addItem(pName);
    }
    shapeLabel = new QLabel(tr("&Shape:"));
    shapeLabel->setBuddy(shapeComboBox);


    penWidthSpinBox = new QSpinBox;
    penWidthSpinBox->setRange(1, 20);
    penWidthSpinBox->setRange(5, 20);
    penWidthLabel = new QLabel(tr("Pen &Width:"));
    penWidthLabel->setBuddy(penWidthSpinBox);


    colorButton = new QPushButton;
    setColor(colorButton,defaultColor);

    colorLabel = new QLabel(tr("&Color:"));
    colorLabel->setBuddy(colorButton);

    backButton = new QPushButton;
    backButton->setText("Undo");

    forwardButton = new QPushButton;
    forwardButton->setText("Redo");

    connect(shapeComboBox, SIGNAL(activated(int)),this, SLOT(shapeChanged()));
    connect(penWidthSpinBox, SIGNAL(valueChanged(int)),this, SLOT(penChanged()));
    connect(colorButton, SIGNAL(clicked()),this, SLOT(colorChanged()));
    connect(backButton, SIGNAL(clicked()),this, SLOT(undo()));
    connect(forwardButton, SIGNAL(clicked()),this, SLOT(redo()));

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->setColumnStretch(3, 3);
    mainLayout->addWidget(shapeLabel, 2, 0, Qt::AlignRight);
    mainLayout->addWidget(shapeComboBox, 2, 1);
    mainLayout->addWidget(penWidthLabel, 3, 0, Qt::AlignRight);
    mainLayout->addWidget(penWidthSpinBox, 3, 1);
    mainLayout->addWidget(colorLabel, 4, 0, Qt::AlignRight);
    mainLayout->addWidget(colorButton,4,1);
    mainLayout->addWidget(backButton,2,2);
    mainLayout->addWidget(forwardButton,3,2);
    setLayout(mainLayout);
    shapeChanged();
    penChanged();
    setWindowTitle(tr("RenderUI"));

}

void Window::shapeChanged()
{
    int curIndex = shapeComboBox->currentIndex();
    QString curShape = classNames[curIndex];
    int id = QMetaType::type(curShape.toStdString().c_str());
    MyShape* shapePtr = (MyShape*) QMetaType::create(id);

    if (renderArea->shapePtr != NULL){
        defaultColor = renderArea->shapePtr->shapeColor;
    }
    renderArea->shapePtr = shapePtr;
    renderArea->shapePtr->shapeColor = defaultColor;
    penChanged();
}

void Window::penChanged()
{
    int width = penWidthSpinBox->value();
    renderArea->shapePtr->lineWidth = width;
}

void Window::colorChanged()
{
    QColor pColor = renderArea->shapePtr->shapeColor;
    QColor color = QColorDialog::getColor(pColor, this);
    if(color.isValid()){
        renderArea->shapePtr->shapeColor = color;
        setColor(colorButton,color);
    }

}

void Window::undo()
{
    renderArea->undo();
}

void Window::redo()
{
    renderArea->redo();
}

void Window::closeEvent(QCloseEvent *e){
    //qDebug()<<"close Window"<< endl;
}




