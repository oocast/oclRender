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

    QVBoxLayout *mainLayout = new QVBoxLayout;
    
    colorDialog = new QColorDialog();
    colorDialog->setWindowFlags(Qt::Widget);
    colorDialog->setOptions(QColorDialog::NoButtons|QColorDialog::ShowAlphaChannel);
    colorDialog->setCurrentColor(defaultColor);
    
    
	shapeButtonGroup = new QButtonGroup;

    for(int i=0;i<classNames.size();i++){
    	QRadioButton *qrb = new QRadioButton;
    	if(i == 0){
    		qrb -> setChecked(true);
    	}
    	QString pName = classNames[i];
    	
		QString curSS="";
        curSS.append("QRadioButton::indicator::checked { image: url(./image/");
        curSS.append(pName);
        curSS.append("_checked.png);}QRadioButton::indicator::unchecked {image: url(./image/");
        curSS.append(pName);
        curSS.append("_unchecked.png);}");
        qrb->setStyleSheet(curSS);
        
    	
    	shapeButtonGroup->addButton(qrb);
    	shapeButtonGroup->setId(qrb,i);
    	mainLayout->addWidget(qrb);
    	connect(qrb, SIGNAL(clicked()),this, SLOT(shapeChanged()));
    }
    shapeButtonGroup->setExclusive(true);
    

    penWidthSpinBox = new QSpinBox;
    penWidthSpinBox->setRange(1, 20);
    penWidthSpinBox->setValue(5);



    backButton = new QPushButton;
    backButton->setText("Undo");

    forwardButton = new QPushButton;
    forwardButton->setText("Redo");

    connect(penWidthSpinBox, SIGNAL(valueChanged(int)),this, SLOT(penChanged()));
    connect(colorDialog, SIGNAL(currentColorChanged(QColor)),this, SLOT(colorChanged()));
    connect(backButton, SIGNAL(clicked()),this, SLOT(undo()));
    connect(forwardButton, SIGNAL(clicked()),this, SLOT(redo()));


    mainLayout->addWidget(penWidthSpinBox);
    mainLayout->addWidget(backButton);
    mainLayout->addWidget(forwardButton);
    mainLayout->addWidget(colorDialog);
    setLayout(mainLayout);
    shapeChanged();
    penChanged();
    setWindowTitle(tr("RenderUI"));

}

void Window::shapeChanged()
{
    int curIndex = shapeButtonGroup->checkedId();
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
    QColor color = colorDialog->currentColor();
    if(color.isValid()){
        renderArea->shapePtr->shapeColor = color;
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




