#pragma once

#include "location.h"
#include <QWidget>
class Client;
class PostDetailsPage;
class PostListPage;
class QSettings;
class QStackedLayout;
namespace Ui { class MainWindow; }

class MainWindow: public QWidget
{
    Q_OBJECT

  private:
    Ui::MainWindow* _ui = nullptr;
    QStackedLayout* _layout = nullptr;
    PostListPage* _list_page = nullptr;
    PostDetailsPage* _details_page = nullptr;
    Client* _client = nullptr;
    QSettings* _settings = nullptr;

    Location _home_location;
    Location _current_location;
    void save_locations_to_settings();

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

  public slots:
    void show_list_page();
    void show_details_page(const QString& post_id);

  private slots:
    void initialize_or_quit();
    void show_location_dialog();
};
