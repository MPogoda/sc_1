#include <QApplication>
#include "mainwindow.h"
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a( argc, argv );
    std::unique_ptr< virtualBartender::MainWindow > w{ new virtualBartender::MainWindow{} };

    w->show();
    return a.exec();
}
