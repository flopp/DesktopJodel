#include "locationDialog.h"
#include "ui_locationDialog.h"
#include <QMessageBox>

LocationDialog::LocationDialog(QWidget* parent) : QDialog(parent)
{
  _ui = new Ui::LocationDialog;
  _ui->setupUi(this);

  connect(_ui->loadHomeButton, SIGNAL(clicked()), this, SLOT(load_home_location()));
  connect(_ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(_ui->okButton, SIGNAL(clicked()), this, SLOT(validate_and_accept()));
}


LocationDialog::~LocationDialog()
{
  delete _ui;
}


bool LocationDialog::validate()
{
  QStringList messages;
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


void LocationDialog::load_home_location()
{
  _ui->locationEdit->set_location(_home_location);
}


void LocationDialog::validate_and_accept()
{
  if (validate())
  {
    accept();
  }
}


Location LocationDialog::get_location() const
{
  return _ui->locationEdit->get_location();
}


void LocationDialog::set_location(const Location& location)
{
  _ui->locationEdit->set_location(location);
}


void LocationDialog::set_home_location(const Location& location)
{
  _home_location = location;
}
