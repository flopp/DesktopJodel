#pragma once

#include <QDialog>
namespace Ui { class SettingsDialog; }

class SettingsDialog: public QDialog
{
    Q_OBJECT

  private:
    Ui::SettingsDialog* _ui = nullptr;

  public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    virtual ~SettingsDialog();

    void load_settings(const Client& client);
    void store_settings(Client& client);

  public slots:
    void generate_device_uid();
    void validate();
};
