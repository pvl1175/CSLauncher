#include "widget.h"
#include "shell.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Shell sh;
    sh.show();

    return a.exec();
}
