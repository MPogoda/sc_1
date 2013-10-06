#pragma once

#include <QWidget>

namespace Ui
{
class MainWindow;
} // namespace Ui

namespace virtualBartender
{
class MainWindow : public QWidget
{
public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow();
private:
    Ui::MainWindow *const ui;
}; // class MainWindow
} // namespace virtualBartender
