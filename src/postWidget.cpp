#include "client.h"
#include "emoji.h"
#include "postWidget.h"
#include "ui_postWidget.h"
#include "voteWidget.h"
#include <QPixmap>

PostWidget::PostWidget(QWidget* parent) :
  QWidget(parent)
{
  _ui = new Ui::PostWidget;
  _ui->setupUi(this);
  connect(_ui->votes, SIGNAL(up_vote()), this, SLOT(slotVoteUp()));
  connect(_ui->votes, SIGNAL(down_vote()), this, SLOT(slotVoteDown()));
  connect(_ui->detailsButton, SIGNAL(clicked()), this, SLOT(slotShowDetails()));
  set_post(Post());
}


PostWidget::~PostWidget()
{
  delete _ui;
}


void PostWidget::set_post(const Post& post)
{
  if (post.client() != _post.client())
  {
    if (_post.client()) disconnect(_post.client(), SIGNAL(image_ready(QUrl, QPixmap)), this, SLOT(handle_image(QUrl, QPixmap)));
    if (post.client()) connect(post.client(), SIGNAL(image_ready(QUrl, QPixmap)), this, SLOT(handle_image(QUrl, QPixmap)));
  }
  _post = post;

  _ui->messageLabel->setText(emoji().to_html(_post.get_message()));
  _ui->infoLabel->setText(QString("%1; %2").arg(_post.get_created_at().toString("yyyy-MM-dd hh:mm:ss")).arg(_post.get_city()));
  if (_post.is_created_by_op())
  {
    _ui->infoLabel->setText("OP " + _ui->infoLabel->text());
  }

  set_color(_post.get_color());

  _ui->votes->set_votes(_post.get_votes());
  _ui->votes->set_voted_direction(_post.get_voted());

  display_child_count(_post.get_child_count());
  initiate_image_request(_post.get_image_url());
}


void PostWidget::slotVoteUp()
{
  _post.vote_up();
  slotRefreshPost();
}


void PostWidget::slotVoteDown()
{
  _post.vote_down();
  slotRefreshPost();
}


void PostWidget::slotRefreshPost()
{
  _post.refresh();
  set_post(_post);
}


void PostWidget::toggle_details_button(bool enabled)
{
  _ui->detailsButton->setVisible(enabled);
}


void PostWidget::slotShowDetails()
{
  emit show_details(_post.get_post_id());
}


void PostWidget::handle_image(const QUrl& url, const QPixmap& image)
{
  if (url == _image_url)
  {
    _ui->imageWidget->set_image(image);
  }
}


void PostWidget::set_color(const QColor& color)
{
  const QColor text_color = color.lightnessF() < 0.5 ? Qt::white : Qt::black;
  setStyleSheet(QString("#widget { background-color: %1; color: %2; } QWidget { color: %2; } QPushButton { font: bold; }").arg(color.name(), text_color.name()));
}


void PostWidget::initiate_image_request(const QString& raw_image_url)
{
  _ui->imageWidget->clear_image();
  _image_url.clear();
  if (!raw_image_url.isEmpty() && _post.client())
  {
    if (raw_image_url.startsWith("//")) _image_url = QUrl("http:" + raw_image_url);
    else _image_url = raw_image_url;
    _post.client()->fetch_image(_image_url);
  }
}


void PostWidget::display_child_count(int count)
{
  auto b = _ui->detailsButton;
  if (count == 0)
  {
    b->setText("Details...");
  }
  else if (count == 1)
  {
    b->setText("Details (1 Reply)...");
  }
  else
  {
    b->setText(QString("Details (%1 Replies)...").arg(count));
  }
}
