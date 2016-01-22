#pragma once

#include "location.h"
#include <QDialog>
#include <QString>
namespace Ui { class LocationDialog; }

class LocationDialog : public QDialog
{
    Q_OBJECT

  public:
    LocationDialog(QWidget* parent = nullptr);
    virtual ~LocationDialog();

    Location get_location() const;
    void set_location(const Location& location);
    void set_home_location(const Location& location);

  private slots:
    bool validate();
    void validate_and_accept();
    void load_home_location();

  private:
    Ui::LocationDialog* _ui = nullptr;
    Location _home_location;
};
