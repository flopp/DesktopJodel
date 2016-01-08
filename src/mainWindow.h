#pragma once

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

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

  public slots:
    void showListPage();
    void showDetailsPage(const QString& post_id);

  private slots:
    void initialize_or_quit();
};
