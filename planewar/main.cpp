#include "mainscene.h"

#include <QApplication>
#include <QResource>//注册二进制文件.rcc用

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QResource::registerResource("./resource.rcc");//注册二进制文件.rcc
    Mainscene w;
    w.show();
    return a.exec();
}
