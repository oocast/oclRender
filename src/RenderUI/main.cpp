#include "myshape.h"
#include "renderarea.h"
#include "window.h"
#include <QDebug>
#include <QtWidgets>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    registerAllClasses();



    QProcess *proc;
    proc = new QProcess();
    proc->execute("xdg-open ../RenderUI/testopen");
    QThread::sleep(3);
    proc->execute("wmctrl -r testopen -e 1,400,100,500,400");


    RenderArea* ra = new RenderArea;
    Window window(ra);
    window.move(0,0);
    ra->setGeometry(400,100,500,400);
    ra->setAttribute(Qt::WA_TranslucentBackground, true);
    window.show();
    ra->show();
    return app.exec();
}
