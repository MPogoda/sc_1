#include "parametersdialog.h"
#include "ui_parametersdialog.h"

#include <deque>
#include <memory>

#include <fl/Headers.h>

namespace virtualBartender
{
struct ParametersDialog::ParametersImpl
{
    static uint32_t calcDrunknessMaximum( const int n )
    {
        switch (n) {
            case 0: return 3000;
            case 1: return 5000;
            case 2: default: return 6000;
        }
    }

    ParametersImpl( const int n )
      : drunknessMaximum{ calcDrunknessMaximum( n ) }
      , engine{ new fl::Engine{ "Virtual bartender "}}
      , inputTime{ new fl::InputVariable{ "Time" }}
      , inputNumber{ new fl::InputVariable{ "Number" }}
      , inputDrunkness{ new fl::InputVariable{ "Drunkness" }}
      , outputBeverage{ new fl::OutputVariable{ "Beverage" }}
      , ruleblock{ new fl::RuleBlock{ "Rule block" }}
    {
        createInputTime();
        createInputNumber();
        createInputDrunkness( n );

        createOutputBeverage();
        createRules();

        engine->configure("Minimum", "Maximum", "AlgebraicProduct", "AlgebraicSum", "Centroid");

        std::string status;
        if (!engine->isReady(&status)) {
            throw fl::Exception("Engine not ready. "
                "The following errors were encountered:\n" + status, FL_AT);
        }
    }
    ~ParametersImpl()
    {
        inputTime.release();
        inputNumber.release();
        inputDrunkness.release();
        outputBeverage.release();
    }

    void createInputTime()
    {
        std::unique_ptr< fl::Term > tempTerm;
        //inputTime
        inputTime->setRange(0.000, 6.000);
        tempTerm.reset( new fl::Sigmoid{ "LITTLEBIT", 0.500, -3.333 } );
            inputTime->addTerm( tempTerm.release() );
        tempTerm.reset( new fl::Gaussian{ "NOTMUCH", 1.500, 0.500 } );
            inputTime->addTerm( tempTerm.release() );
        tempTerm.reset( new fl::Bell{ "MEDIUM", 3.500, 1.000, 4.000 } );
            inputTime->addTerm( tempTerm.release() );
        tempTerm.reset( new fl::Sigmoid{ "ALOT", 4.000, 3.500 } );
            inputTime->addTerm( tempTerm.release() );

        engine->addInputVariable(inputTime.get());
    }

    void createInputNumber()
    {
        std::unique_ptr< fl::Term > tempTerm;
        //inputNumber
        inputNumber->setRange(0.000, 8.000);

        tempTerm.reset( new fl::Ramp{ "FEW", 4.000, 0.000 } );
            inputNumber->addTerm( tempTerm.release() );
        tempTerm.reset( new fl::Gaussian{ "GROUP", 4.000, 1.000 } );
            inputNumber->addTerm( tempTerm.release() );
        tempTerm.reset( new fl::Ramp{ "BUNCH", 4.000, 8.000 } );
            inputNumber->addTerm( tempTerm.release() );

        engine->addInputVariable(inputNumber.get());
    }

    void createInputDrunkness( const int n )
    {
        std::unique_ptr< fl::Term > tempTerm;
        inputDrunkness->setRange(0.000, 6000.000);

        switch( n ) {
            case 0:
              tempTerm.reset( new fl::Triangle{ "SOBER", 0.000, 500.000, 1000.000 } );
                  inputDrunkness->addTerm( tempTerm.release() );
              tempTerm.reset( new fl::Triangle{ "TIPSY", 500.000, 1750.000, 3000.000 } );
                  inputDrunkness->addTerm( tempTerm.release() );
              tempTerm.reset( new fl::Sigmoid{ "DRUNK", 3000.000, 0.002 } );
                  inputDrunkness->addTerm( tempTerm.release() );
              break;
            case 1:
              tempTerm.reset( new fl::Trapezoid{ "SOBER", 0.0, 900.0, 1700.0, 2000.0 } );
                  inputDrunkness->addTerm( tempTerm.release() );
              tempTerm.reset( new fl::Gaussian{ "TIPSY", 2400.0, 420.0 } );
                  inputDrunkness->addTerm( tempTerm.release() );
              tempTerm.reset( new fl::Sigmoid{ "DRUNK", 2500.0, 0.001 } );
                  inputDrunkness->addTerm( tempTerm.release() );
              break;
            case 2:
            default:
              tempTerm.reset( new fl::Trapezoid{ "SOBER", 0.0, 900.0, 1700.0, 2960.0 } );
                  inputDrunkness->addTerm( tempTerm.release() );
              tempTerm.reset( new fl::Gaussian{ "TIPSY", 3120.0, 1020.0 } );
                  inputDrunkness->addTerm( tempTerm.release() );
              tempTerm.reset( new fl::Sigmoid{ "DRUNK", 5020.0, 0.009 } );
                  inputDrunkness->addTerm( tempTerm.release() );
              break;
        }

        engine->addInputVariable( inputDrunkness.get());
    }

    void createOutputBeverage()
    {
        std::unique_ptr< fl::Term > tempTerm;
        outputBeverage->setRange(250.000, 1500.000);
        outputBeverage->setLockOutputRange(false);
        outputBeverage->setDefaultValue(0.000);
        outputBeverage->setLockValidOutput(false);
        outputBeverage->setDefuzzifier(new fl::Centroid( 9000 ));
        outputBeverage->output()->setAccumulation(new fl::Maximum);

        tempTerm.reset( new fl::Sigmoid{ "SMALL", 330.000, -0.014 } );
            outputBeverage->addTerm( tempTerm.release() );
        tempTerm.reset( new fl::Gaussian{ "MEDIUM", 600.000, 100.000 } );
            outputBeverage->addTerm( tempTerm.release() );
        tempTerm.reset( new fl::Gaussian{ "BIG", 900.000, 150.000 } );
            outputBeverage->addTerm( tempTerm.release() );
        tempTerm.reset( new fl::Sigmoid{ "HUGE", 1100.000, 0.020 } );
            outputBeverage->addTerm( tempTerm.release() );

        engine->addOutputVariable(outputBeverage.get());
    }

    void createRules()
    {
        ruleblock->setTnorm(new fl::Minimum);
        ruleblock->setSnorm(new fl::Maximum);
        ruleblock->setActivation(new fl::Minimum);

        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is SOBER and Time is ALOT then Beverage is BIG", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is SOBER and Time is MEDIUM and Number is FEW then Beverage is BIG", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is SOBER and Time is MEDIUM and Number is GROUP then Beverage is HUGE", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is SOBER and Time is MEDIUM and Number is BUNCH then Beverage is HUGE", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is SOBER and Time is NOTMUCH and Number is FEW then Beverage is MEDIUM", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is SOBER and Time is NOTMUCH and Number is GROUP then Beverage is MEDIUM", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is SOBER and Time is NOTMUCH and Number is BUNCH then Beverage is BIG", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is SOBER and Time is LITTLEBIT and Number is FEW then Beverage is MEDIUM", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is SOBER and Time is LITTLEBIT and Number is GROUP then Beverage is BIG", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is SOBER and Time is LITTLEBIT and Number is BUNCH then Beverage is BIG", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is TIPSY and Time is LITTLEBIT then Beverage is MEDIUM", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is TIPSY and Time is NOTMUCH and Number is FEW then Beverage is MEDIUM", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is TIPSY and Time is NOTMUCH and Number is GROUP then Beverage is BIG", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is TIPSY and Time is NOTMUCH and Number is BUNCH then Beverage is HUGE", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is TIPSY and Time is MEDIUM and Number is FEW then Beverage is MEDIUM", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is TIPSY and Time is MEDIUM and Number is GROUP then Beverage is BIG", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is TIPSY and Time is MEDIUM and Number is BUNCH then Beverage is BIG", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is TIPSY and Time is ALOT and Number is FEW then Beverage is SMALL", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is TIPSY and Time is ALOT and Number is GROUP then Beverage is MEDIUM", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is TIPSY and Time is ALOT and Number is BUNCH then Beverage is BIG", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is DRUNK and Time is LITTLEBIT then Beverage is SMALL", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is DRUNK and Time is NOTMUCH and Number is FEW then Beverage is SMALL", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is DRUNK and Time is NOTMUCH and Number is GROUP then Beverage is SMALL", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is DRUNK and Time is NOTMUCH and Number is BUNCH then Beverage is MEDIUM", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is DRUNK and Time is MEDIUM and Number is FEW then Beverage is SMALL", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is DRUNK and Time is MEDIUM and Number is GROUP then Beverage is MEDIUM", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is DRUNK and Time is MEDIUM and Number is BUNCH then Beverage is MEDIUM", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is DRUNK and Time is ALOT and Number is FEW then Beverage is SMALL", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is DRUNK and Time is ALOT and Number is GROUP then Beverage is BIG", engine.get()));
        ruleblock->addRule(fl::FuzzyRule::parse("if Drunkness is DRUNK and Time is ALOT and Number is BUNCH then Beverage is BIG ", engine.get()));

        engine->addRuleBlock(ruleblock.release());
    }

    uint32_t drunknessMaximum;
    std::unique_ptr< fl::Engine > engine;
    std::unique_ptr< fl::InputVariable > inputTime;
    std::unique_ptr< fl::InputVariable > inputNumber;
    std::unique_ptr< fl::InputVariable > inputDrunkness;
    std::unique_ptr< fl::OutputVariable > outputBeverage;
    std::unique_ptr< fl::RuleBlock > ruleblock;
}; // struct ParametersDialog::ParametersImpl

ParametersDialog::ParametersDialog( QWidget *const parent )
  : QDialog{ parent }
  , impl_{ nullptr }
  , ui_{ new Ui::ParametersDialog{} }
{
    if (ui_) {
        ui_->setupUi( this );
    }
}

ParametersDialog::~ParametersDialog()
{
    delete ui_;
}

uint32_t ParametersDialog::getGroupCount() const
{
    return ui_->group->value();
}

uint32_t ParametersDialog::getHours() const
{
    return ui_->time->value();
}

uint32_t ParametersDialog::getDrunknessMaximum() const
{
    if (!impl_) impl_.reset( new ParametersImpl{ ui_->drunk->value() });

    return impl_->drunknessMaximum;
}

uint32_t
ParametersDialog::calculateBeverage( uint32_t minutes
                                   , uint32_t group
                                   , uint32_t drunk ) const
{
    impl_->inputTime->setInput( minutes );
    impl_->inputNumber->setInput( group );
    impl_->inputDrunkness->setInput( drunk );
    impl_->engine->process();
    return impl_->outputBeverage->defuzzify();
}
} // namespace virtualBartender
