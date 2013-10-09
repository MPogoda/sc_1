#include "parametersdialog.h"
#include "ui_parametersdialog.h"
#include <fl/Engine.h>

namespace virtualBartender
{
struct ParametersDialog::ParametersImpl
{
    ParametersImpl()
      : drunknessMaximum{ 6000 }
      , engine{ new fl::Engine{}}
    {
        // Create basics of engine.
    }
    uint32_t drunknessMaximum;
    std::unique_ptr< fl::Engine > engine;
}; // struct ParametersDialog::ParametersImpl

ParametersDialog::ParametersDialog( QWidget *const parent )
  : QDialog{ parent }
  , impl_{ new ParametersImpl{} }
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
    // Use slider, luke.
    // TODO: add terms to engine for drunkness
    // TODO: return some maximum value
    // ui_->drunk->value();
    return impl_->drunknessMaximum;
}

uint32_t
ParametersDialog::calculateBeverage( uint32_t minutes
                                   , uint32_t group
                                   , uint32_t drunk ) const
{
    // TODO USE FUZZY ENGINE
    return 300;
}
} // namespace virtualBartender
