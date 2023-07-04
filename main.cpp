#include "gamewindow.h"
#include "selectionwindow.h"
#include "startwindow.h"
#include <note.h>

#include <QApplication>

QWidget* currentWindow;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    currentWindow=new StartWindow(nullptr,true);
    currentWindow->show();
    return a.exec();
}
