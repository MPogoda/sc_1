#pragma once

#include <QWidget>
#include <chrono>

namespace Ui
{
class MainWindow;
} // namespace Ui

namespace virtualBartender
{
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow();
private:
    uint32_t drinkLeft_;
    std::chrono::seconds timeForMl_;

    Ui::MainWindow *const ui_;
private slots:
    void Drink();
    void Order();
}; // class MainWindow
} // namespace virtualBartender
