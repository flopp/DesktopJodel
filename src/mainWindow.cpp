#include "client.h"
#include "mainWindow.h"
#include "postDetailsPage.h"
#include "postListPage.h"
#include "settingsDialog.h"
#include "ui_mainWindow.h"
#include <QMessageBox>
#include <QSettings>
#include <QStackedLayout>

MainWindow::MainWindow(QWidget* parent) :
  QWidget(parent)
{
  _settings = new QSettings(this);
  _client = new Client(
      _settings->value("device_uid", Client::randomDeviceUid()).toString(),
      _settings->value("city", "Berlin").toString(),
      _settings->value("country_code", "DE").toString(),
      _settings->value("lat", 52.516667).toDouble(),
      _settings->value("lng", 13.383333).toDouble());

  _ui = new Ui::MainWindow;
  _ui->setupUi(this);

  _layout = new QStackedLayout(this);

  _list_page = new PostListPage(this);
  _list_page->set_client(_client);
  _layout->addWidget(_list_page);
  connect(_list_page, SIGNAL(show_post_details(QString)), this, SLOT(showDetailsPage(QString)));
  connect(_list_page, SIGNAL(show_settings()), this, SLOT(showSettingsDialog()));

  _details_page = new PostDetailsPage(this);
  _details_page->set_client(_client);
  _layout->addWidget(_details_page);
  connect(_details_page, SIGNAL(back_to_list()), this, SLOT(showListPage()));

  showListPage();
}


MainWindow::~MainWindow()
{
  delete _ui;
  delete _client;
}


void MainWindow::showListPage()
{
  _layout->setCurrentWidget(_list_page);
}


void MainWindow::showDetailsPage(const QString& post_id)
{
  _layout->setCurrentWidget(_details_page);
  _details_page->load_post(post_id);
}


void MainWindow::showSettingsDialog()
{
  if (!_settingsDialog)
  {
    _settingsDialog = new SettingsDialog(this);
    connect(_settingsDialog, SIGNAL(accepted()), this, SLOT(store_settings_from_dialog()));
  }

  _settingsDialog->load_settings(*_client);
  _settingsDialog->show();
}


void MainWindow::store_settings_from_dialog()
{
  Q_ASSERT(_settingsDialog);
  _settingsDialog->store_settings(*_client);
  _settings->setValue("device_uid", _client->get_device_uid());
  _settings->setValue("city", _client->get_city());
  _settings->setValue("country_code", _client->get_country_code());
  _settings->setValue("lat", _client->get_lat());
  _settings->setValue("lng", _client->get_lng());
  _settings->sync();

  QMessageBox::information(this, "Info", "Setting changed. Closing application now.");

  qApp->quit();
}
