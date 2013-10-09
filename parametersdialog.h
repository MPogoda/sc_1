#pragma once

#include <QDialog>
#include <memory>

namespace Ui
{
struct ParametersDialog;
} // namespace Ui

namespace virtualBartender
{
class ParametersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ParametersDialog( QWidget *parent = nullptr );
    ~ParametersDialog();

    uint32_t getHours() const;
    uint32_t getDrunknessMaximum() const;
    uint32_t getGroupCount() const;

    uint32_t calculateBeverage( uint32_t minutes, uint32_t group, uint32_t drunk ) const;
private:
    struct ParametersImpl;
    std::unique_ptr< ParametersImpl > impl_;
    Ui::ParametersDialog* ui_;
}; // class ParametersDialog
} // namespace virtualBartender
