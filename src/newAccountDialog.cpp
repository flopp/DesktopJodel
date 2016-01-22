#include "newAccountDialog.h"
#include "ui_newAccountDialog.h"
#include <QMessageBox>

NewAccountDialog::NewAccountDialog(QWidget* parent) : QDialog(parent)
{
  _ui = new Ui::NewAccountDialog;
  _ui->setupUi(this);

  connect(_ui->deviceUidEdit, SIGNAL(textChanged(QString)), this, SLOT(update_device_uid_label(QString)));
  connect(_ui->generateButton, SIGNAL(clicked()), this, SLOT(create_random_device_uid()));
  connect(_ui->exitButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(_ui->okButton, SIGNAL(clicked()), this, SLOT(validate_and_accept()));

  create_random_device_uid();
}


NewAccountDialog::~NewAccountDialog()
{
  delete _ui;
}


QString NewAccountDialog::get_device_uid() const
{
  return _ui->deviceUidEdit->text().toLower();
}


Location NewAccountDialog::get_location() const
{
  return _ui->locationEdit->get_location();
}


void NewAccountDialog::set_location(const Location& location)
{
  _ui->locationEdit->set_location(location);
}


bool NewAccountDialog::validate()
{
  QStringList messages;
  if (_ui->deviceUidEdit->text().size() != 64)
  {
    messages << "The 'Device UID' must contain exactly 64 alpha-numeric characters.";
  }

  const Location location = _ui->locationEdit->get_location();
  if (location.city.isEmpty())
  {
    messages << "'City' must not be empty.";
  }

  if (!messages.isEmpty())
  {
    QMessageBox::warning(this, "Errors in Input", messages.join("\n"));
  }

  return messages.isEmpty();
}


void NewAccountDialog::create_random_device_uid()
{
  static QString chars = "abcdefghijklmnopqrstuvwxyz0123456789";
  const int random_string_length = 64;

  QString random_string;
  for (int i = 0; i < random_string_length; ++i)
  {
    const int index = qrand() % chars.length();
    random_string += chars[index];
  }

  _ui->deviceUidEdit->setText(random_string);
}


void NewAccountDialog::validate_and_accept()
{
  if (validate())
  {
    accept();
  }
}


void NewAccountDialog::update_device_uid_label(const QString& device_uid)
{
  _ui->deviceUidLabel->setText(QString("%1/64 characters").arg(device_uid.size()));
}
