#include "client.h"
#include "locationDialog.h"
#include "mainWindow.h"
#include "newAccountDialog.h"
#include "postDetailsPage.h"
#include "postListPage.h"
#include "ui_mainWindow.h"
#include <QMessageBox>
#include <QSettings>
#include <QStackedLayout>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent) :
  QWidget(parent)
{
  _settings = new QSettings(this);

  _ui = new Ui::MainWindow;
  _ui->setupUi(this);

  qApp->setStyleSheet("QPushButton { font: bold; border-radius: 4px; padding: 4px; outline: none; background-color: palette(dark); color: white; } QPushButton:!enabled { color: #CCCCCC; } QPushButton:focus {}");

  _layout = new QStackedLayout(this);

  _list_page = new PostListPage(this);
  _layout->addWidget(_list_page);
  connect(_list_page, SIGNAL(show_post_details(QString)), this, SLOT(show_details_page(QString)));
  connect(_list_page, SIGNAL(show_location_dialog()), this, SLOT(show_location_dialog()));

  _details_page = new PostDetailsPage(this);
  _layout->addWidget(_details_page);
  connect(_details_page, SIGNAL(back_to_list()), this, SLOT(show_list_page()));

  show_list_page();

  QTimer::singleShot(0, this, SLOT(initialize_or_quit()));
}


MainWindow::~MainWindow()
{
  delete _ui;
  delete _client;
}


void MainWindow::show_list_page()
{
  _layout->setCurrentWidget(_list_page);
}


void MainWindow::show_details_page(const QString& post_id)
{
  _details_page->load_post(post_id);
  _layout->setCurrentWidget(_details_page);
}


void MainWindow::initialize_or_quit()
{
  Q_ASSERT(!_client);

  QString device_uid = _settings->value("device_uid").toString();
  if (device_uid.size() == 64)
  {
    _home_location.city = _settings->value("city", "Berlin").toString();
    _home_location.country = _settings->value("country_code", "DE").toString();
    _home_location.lat = _settings->value("lat", 52.516667).toDouble();
    _home_location.lng = _settings->value("lng", 13.383333).toDouble();

    _current_location.city = _settings->value("current_city", _home_location.city).toString();
    _current_location.country = _settings->value("current_country_code", _home_location.country).toString();
    _current_location.lat = _settings->value("current_lat", _home_location.lat).toDouble();
    _current_location.lng = _settings->value("current_lng", _home_location.lng).toDouble();

    _client = new Client(device_uid, _home_location);
    _client->set_location(_current_location);
  }
  else
  {
    NewAccountDialog d;
    d.set_location(Location("Berlin", "DE", 52.516667, 13.383333));
    if (d.exec() != QDialog::Accepted)
    {
      qApp->quit();
    }

    _home_location = d.get_location();
    _current_location = _home_location;
    _client = new Client(d.get_device_uid(), _home_location);
  }

  _settings->setValue("device_uid", _client->get_device_uid());
  save_locations_to_settings();
  _settings->sync();

  try
  {
    _client->authenticate();
  }
  catch(...)
  {
    // TODO: show error message
  }

  _list_page->set_client(_client);
  _details_page->set_client(_client);

  _list_page->refresh_list();
}


void MainWindow::show_location_dialog()
{
  LocationDialog d;
  d.set_home_location(_home_location);
  d.set_location(_current_location);

  if (d.exec() != QDialog::Accepted)
  {
    return;
  }

  _current_location = d.get_location();
  _client->set_location(_current_location);

  save_locations_to_settings();
}


void MainWindow::save_locations_to_settings()
{
  _settings->setValue("city", _home_location.city);
  _settings->setValue("country_code", _home_location.country);
  _settings->setValue("lat", _home_location.lat);
  _settings->setValue("lng", _home_location.lng);

  _settings->setValue("current_city", _current_location.city);
  _settings->setValue("current_country_code", _current_location.country);
  _settings->setValue("current_lat", _current_location.lat);
  _settings->setValue("current_lng", _current_location.lng);

  _settings->sync();
}
