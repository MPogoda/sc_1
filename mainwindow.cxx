#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>

#include <iostream>

namespace virtualBartender
{
MainWindow::MainWindow( QWidget *parent )
  : QWidget{ parent }
  , ui_{ new Ui::MainWindow{} }
{
    if (ui_) {
        ui_->setupUi( this );
    }

    ui_->order_b->setEnabled( true );

    connect( ui_->order_b, SIGNAL(clicked()), this, SLOT(Order()));
    connect( ui_->drink_b, SIGNAL(clicked()), this, SLOT(Drink()));

    // TODO: remove this
    ui_->time->setValue( 30 );
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::Order()
{
    using namespace std::chrono;
    constexpr minutes timeForDrink{ 30 };

    // TODO: CALCULATE
    const uint32_t ml{ 300 };

    if ( 50 > ml ) {
        // well, shit
        // No more, bro
    }

    timeForMl_ = duration_cast< seconds >( timeForDrink / static_cast< double >( ml ) );

    ui_->beverage->setMaximum( ml );
    ui_->beverage->setValue( ml );

    ui_->order_b->setEnabled( false );
    ui_->beverage_gb->setEnabled( true );
    ui_->company_gb->setEnabled( false );
}

void MainWindow::Drink()
{
    using namespace std;
    random_device rd{};
    mt19937 generator{ rd() };
    uniform_int_distribution<int32_t> int_dist{ 0, ui_->beverage->maximum() };

    const int32_t will_drink{ qMin( int_dist( generator)
                                  , ui_->beverage->value() ) };
    cout << will_drink << "\n";
    // TODO: Calculate drunkness

    // Re-calculating time
    using namespace std::chrono;
    const minutes timeElapsed = duration_cast< minutes >( timeForMl_ * will_drink );
    cout << timeElapsed.count() << "m\n";
    ui_->time->setValue( ui_->time->value() - timeElapsed.count() );

    // Sipping beverage
    ui_->beverage->setValue( ui_->beverage->value() - will_drink );

    if (0 == ui_->beverage->value()) {
        ui_->beverage_gb->setEnabled( false );

        if (30 > ui_->time->value()) {
            ui_->time->setValue( 0 );
            // Sorry bro
        } else {
            ui_->order_b->setEnabled( true );
            ui_->company_gb->setEnabled( true );
        }
    }
}
} // namespace virtualBartender
