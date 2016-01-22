#pragma once

#include <QWidget>
namespace Ui { class PostListPage; }
namespace qjodel { class Client; }
class PostWidget;
class QVBoxLayout;
class QSpacerItem;

class PostListPage: public QWidget
{
    Q_OBJECT

  private:
    Ui::PostListPage* _ui = nullptr;
    Client* _client = nullptr;
    QList<PostWidget*> _post_widgets;
    QVBoxLayout* _layout = nullptr;
    QSpacerItem* _spacer = nullptr;

  public:
    explicit PostListPage(QWidget* parent = nullptr);
    virtual ~PostListPage();

    void set_client(Client* client);

  public slots:
    void refresh_list();
    void update_karma();
    void post_message();

  signals:
    void show_post_details(const QString& post_id);
    void show_location_dialog();
};
