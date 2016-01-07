#pragma once

#include <QWidget>
class Client;
class PostWidget;
class QSpacerItem;
class QVBoxLayout;
namespace Ui { class PostDetailsPage; }

class PostDetailsPage: public QWidget
{
    Q_OBJECT

  private:
    Ui::PostDetailsPage* _ui = nullptr;
    Client* _client = nullptr;
    Post _post;
    QList<PostWidget*> _post_widgets;
    QVBoxLayout* _layout = nullptr;
    QSpacerItem* _spacer = nullptr;
    void update_post();

  public:
    explicit PostDetailsPage(QWidget* parent = nullptr);
    virtual ~PostDetailsPage();

    void set_client(Client* client);
    void load_post(const QString& post_id);

  public slots:
    void send_reply();

  signals:
    void back_to_list();
};
