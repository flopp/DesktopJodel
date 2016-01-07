#include "client.h"
#include "settingsDialog.h"
#include "ui_settingsDialog.h"

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent)
{
  _ui = new Ui::SettingsDialog;
  _ui->setupUi(this);

  connect(_ui->generateButton, SIGNAL(clicked()), this, SLOT(generate_device_uid()));
  connect(_ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(_ui->okButton, SIGNAL(clicked()), this, SLOT(validate()));
}


SettingsDialog::~SettingsDialog()
{
  delete _ui;
}


void SettingsDialog::load_settings(const Client& client)
{
  _ui->deviceUidEdit->setText(client.get_device_uid());
  _ui->cityEdit->setText(client.get_city());
  _ui->countryEdit->setText(client.get_country_code());
  _ui->latEdit->setText(QString::number(client.get_lat()));
  _ui->lngEdit->setText(QString::number(client.get_lng()));
}


void SettingsDialog::store_settings(Client& client)
{
  client.set_device_uid(_ui->deviceUidEdit->text());
  client.set_location(_ui->cityEdit->text(), _ui->countryEdit->text(), _ui->latEdit->text().toDouble(), _ui->lngEdit->text().toDouble());
}


void SettingsDialog::generate_device_uid()
{
  _ui->deviceUidEdit->setText(Client::randomDeviceUid());
}


void SettingsDialog::validate()
{
  accept();
}
