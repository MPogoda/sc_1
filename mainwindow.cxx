#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace virtualBartender
{
MainWindow::MainWindow( QWidget *parent )
  : QWidget{ parent }
  , ui{ new Ui::MainWindow{} }
{
    if (ui) {
        ui->setupUi( this );
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
} // namespace virtualBartender
