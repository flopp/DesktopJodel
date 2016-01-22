#pragma once

#include "location.h"
#include <QDialog>
#include <QString>
namespace Ui { class NewAccountDialog; }

class NewAccountDialog: public QDialog
{
    Q_OBJECT

  public:
    NewAccountDialog(QWidget* parent = nullptr);
    virtual ~NewAccountDialog();

    QString get_device_uid() const;
    Location get_location() const;
    void set_location(const Location& location);

  private slots:
    bool validate();
    void validate_and_accept();
    void create_random_device_uid();
    void update_device_uid_label(const QString& device_uid);

  private:
    Ui::NewAccountDialog* _ui = nullptr;
};
