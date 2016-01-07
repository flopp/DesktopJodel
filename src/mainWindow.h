#pragma once

#include <QWidget>
class Client;
class PostDetailsPage;
class PostListPage;
class QSettings;
class QStackedLayout;
class SettingsDialog;
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
    SettingsDialog* _settingsDialog = nullptr;
    QSettings* _settings = nullptr;

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

  public slots:
    void showListPage();
    void showDetailsPage(const QString& post_id);
    void showSettingsDialog();

  private slots:
    void store_settings_from_dialog();
};
