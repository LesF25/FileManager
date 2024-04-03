#include "proxy.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    IService* service = new Proxy(nullptr);
    service->start();

    return a.exec();
}
