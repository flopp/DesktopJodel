#pragma once

#include "post.h"
#include <QList>
#include <QUrl>
#include <QWidget>
class QPixmap;
namespace Ui { class PostWidget; }

class PostWidget: public QWidget
{
    Q_OBJECT

  private:
    Ui::PostWidget* _ui;
    Post _post;
    QUrl _image_url;

  public:
    PostWidget(QWidget* parent = nullptr);
    virtual ~PostWidget();

    void set_post(const Post& post);
    void toggle_details_button(bool enabled);

  public slots:
    void slotVoteUp();
    void slotVoteDown();
    void slotRefreshPost();
    void slotShowDetails();

  private slots:
    void handle_image(const QUrl& url, const QPixmap& image);

  signals:
    void show_details(const QString& post_id);

  private:
    void set_color(const QColor& color);
    void initiate_image_request(const QString& raw_image_url);
    void display_child_count(int count);
};
