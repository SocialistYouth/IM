#include "ckernel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CKernel kernel;


    return a.exec();
}
