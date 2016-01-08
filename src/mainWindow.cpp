#include "client.h"
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

  _layout = new QStackedLayout(this);

  _list_page = new PostListPage(this);
  _layout->addWidget(_list_page);
  connect(_list_page, SIGNAL(show_post_details(QString)), this, SLOT(showDetailsPage(QString)));

  _details_page = new PostDetailsPage(this);
  _layout->addWidget(_details_page);
  connect(_details_page, SIGNAL(back_to_list()), this, SLOT(showListPage()));

  showListPage();

  QTimer::singleShot(0, this, SLOT(initialize_or_quit()));
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


void MainWindow::initialize_or_quit()
{
  Q_ASSERT(!_client);

  QString device_uid = _settings->value("device_uid").toString();
  if (device_uid.size() == 64)
  {
    _client = new Client(
        device_uid,
        _settings->value("city", "Berlin").toString(),
        _settings->value("country_code", "DE").toString(),
        _settings->value("lat", 52.516667).toDouble(),
        _settings->value("lng", 13.383333).toDouble());
  }
  else
  {
    NewAccountDialog d;
    if (d.exec() != QDialog::Accepted)
    {
      qApp->quit();
    }

    _client = new Client(
        d.get_device_uid(),
        d.get_city(),
        d.get_country_code(),
        d.get_lat(),
        d.get_lng());
  }

  _settings->setValue("device_uid", _client->get_device_uid());
  _settings->setValue("city", _client->get_city());
  _settings->setValue("country_code", _client->get_country_code());
  _settings->setValue("lat", _client->get_lat());
  _settings->setValue("lng", _client->get_lng());
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
