#include "client.h"
#include "postDetailsPage.h"
#include "postWidget.h"
#include "ui_postDetailsPage.h"

PostDetailsPage::PostDetailsPage(QWidget* parent) :
  QWidget(parent)
{
  _ui = new Ui::PostDetailsPage;
  _ui->setupUi(this);
  _ui->splitter->setStretchFactor(0, 1);
  _ui->splitter->setStretchFactor(1, 0);
  _ui->splitter->setSizes({10, 1});

  _layout = new QVBoxLayout(_ui->postsWidget);
  _layout->setContentsMargins(0, 0, 0, 0);
  _spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  _layout->addSpacerItem(_spacer);

  connect(_ui->backButton, SIGNAL(clicked()), this, SIGNAL(back_to_list()));
  connect(_ui->replyButton, SIGNAL(clicked()), this, SLOT(send_reply()));
}


PostDetailsPage::~PostDetailsPage()
{
  delete _ui;
}


void PostDetailsPage::set_client(Client* client)
{
  _client = client;
}


void PostDetailsPage::load_post(const QString& post_id)
{
  if (!_client) { return; }

  try
  {
    _post = _client->fetch_post(post_id);
  }
  catch (...)
  {
    // TODO show error message
  }

  update_post();
}


void PostDetailsPage::update_post()
{
  const QList<Post> posts = _post.get_children();


  while (_post_widgets.size() < posts.size() + 1)
  {
    PostWidget* w = new PostWidget;
    w->toggle_details_button(false);
    _layout->insertWidget(_post_widgets.size(), w);
    _post_widgets << w;
  }
  while (_post_widgets.size() > posts.size() + 1)
  {
    delete _post_widgets.last();
    _post_widgets.pop_back();
  }

  Q_ASSERT(_post_widgets.size() >= 1);
  _post_widgets.first()->set_post(_post);

  for (int i = 0; i < posts.size(); ++i)
  {
    _post_widgets[i + 1]->set_post(posts[i]);
  }
}


void PostDetailsPage::send_reply()
{
  if (!_client || !_client->is_authenticated()) return;

  const QString message = _ui->replyText->toPlainText();
  if (message.isEmpty()) return;

  try
  {
    _client->send_reply(message, _post.get_post_id(), _post.get_color_string());
    _ui->replyText->clear();
    load_post(_post.get_post_id());
  }
  catch(...)
  {
    // TODO: display error message
  }
}
