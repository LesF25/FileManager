#include "Proxy.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Proxy proxy;
    proxy.start();

    return a.exec();
}
