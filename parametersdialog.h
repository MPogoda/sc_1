#pragma once

#include <QDialog>
#include <memory>

namespace virtualBartender
{
class ParametersDialog : public QDialog
{
    Q_OBJECT
public:
private:
    struct ParametersImpl;
    std::unique_ptr< ParametersImpl > impl;

}; // class ParametersDialog
} // namespace virtualBartender
