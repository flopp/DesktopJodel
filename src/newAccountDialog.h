#pragma once

namespace Ui { class NewAccountDialog; }
#include <QDialog>
#include <QString>

class NewAccountDialog: public QDialog
{
    Q_OBJECT

  public:
    NewAccountDialog(QWidget* parent = nullptr);
    virtual ~NewAccountDialog();

    QString get_device_uid() const;
    QString get_city() const;
    QString get_country_code() const;
    double get_lat() const;
    double get_lng() const;

  private slots:
    bool validate();
    void validate_and_accept();
    void create_random_device_uid();
    void update_device_uid_label(const QString& device_uid);

  private:
    void fill_country_box();

  private:
    Ui::NewAccountDialog* _ui = nullptr;
};
