#include <QtGui/QApplication>
#include "reversi.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Reversi w;
    w.show();

    return a.exec();
}
