#pragma once

#include "location.h"
#include <QWidget>
namespace Ui { class LocationWidget; }

class LocationWidget: public QWidget
{
    Q_OBJECT

  public:
    LocationWidget(QWidget* parent = nullptr);
    virtual ~LocationWidget();

    Location get_location() const;
    void set_location(const Location& location);
    void set_home_location(const Location& location);

  private:
    void fill_country_box();
    void set_country(const QString& country_code);

  private:
    Ui::LocationWidget* _ui = nullptr;
};
