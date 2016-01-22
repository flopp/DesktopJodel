#include "client.h"
#include "postListPage.h"
#include "postWidget.h"
#include "ui_postListPage.h"
#include <QSpacerItem>
#include <QVBoxLayout>

PostListPage::PostListPage(QWidget* parent) :
  QWidget(parent)
{
  _ui = new Ui::PostListPage;
  _ui->setupUi(this);
  _ui->splitter->setStretchFactor(0, 1);
  _ui->splitter->setStretchFactor(1, 0);
  _ui->splitter->setSizes({10, 1});

  static QList<QString> colors = { "FF9908", "FFBA00", "DD5F5F", "06A3CB", "8ABDB0", "9EC41C" };
  _ui->colorWidget->set_colors(colors);

  _layout = new QVBoxLayout(_ui->postsWidget);
  _layout->setContentsMargins(0, 0, 0, 0);
  _spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  _layout->addSpacerItem(_spacer);

  connect(_ui->refreshListButton, SIGNAL(clicked()), this, SLOT(refresh_list()));
  connect(_ui->postButton, SIGNAL(clicked()), this, SLOT(post_message()));
  connect(_ui->setLocationButton, SIGNAL(clicked()), this, SIGNAL(show_location_dialog()));
}


PostListPage::~PostListPage()
{
  delete _ui;
  qDeleteAll(_post_widgets);
}


void PostListPage::set_client(Client* client)
{
  Q_ASSERT(client);
  _client = client;

  if (_client->is_authenticated())
  {
    _ui->refreshListButton->setText("Refresh");
    _ui->postText->setEnabled(true);
    _ui->postButton->setEnabled(true);
  }
  else
  {
    _ui->refreshListButton->setText("Authenticate");
    _ui->postText->setEnabled(false);
    _ui->postButton->setEnabled(false);
  }
}


void PostListPage::refresh_list()
{
  if (!_client) return;

  if (!_client->is_authenticated())
  {
    try
    {
      _client->authenticate();
    }
    catch (...)
    {
      // TODO: display error
    }

    if (_client->is_authenticated())
    {
      _ui->refreshListButton->setText("Refresh");
      _ui->postText->setEnabled(true);
      _ui->postButton->setEnabled(true);
    }
    else
    {
      _ui->refreshListButton->setText("Authenticate");
      _ui->postText->setEnabled(false);
      _ui->postButton->setEnabled(false);
    }
  }
  else
  {
    QList<Post> posts;
    try
    {
      posts = _client->get_local_posts();
    }
    catch (...)
    {
      // TODO: display error
    }

    while (_post_widgets.size() < posts.size())
    {
      PostWidget* w = new PostWidget;
      _layout->insertWidget(_post_widgets.size(), w);
      _post_widgets << w;
      connect(w, SIGNAL(show_details(QString)), this, SIGNAL(show_post_details(QString)));
    }
    while (_post_widgets.size() > posts.size())
    {
      delete _post_widgets.last();
      _post_widgets.pop_back();
    }

    for (int i = 0; i < posts.size(); ++i)
    {
      _post_widgets[i]->set_post(posts[i]);
    }

    _ui->postsScrollArea->ensureVisible(0, 0);

    update_karma();
  }
}


void PostListPage::update_karma()
{
  if (!_client || !_client->is_authenticated()) return;

  const int karma = _client->get_karma();
  _ui->karmaLabel->setText(QString(karma > 0 ? "+%1" : "%1").arg(karma));
}


void PostListPage::post_message()
{
  if (!_client || !_client->is_authenticated()) return;

  const QString message = _ui->postText->toPlainText();
  if (message.isEmpty()) return;

  try
  {
    _client->send_post(message, _ui->colorWidget->selected_color());
    _ui->postText->clear();
    refresh_list();
  }
  catch(...)
  {
    // TODO: display error message
  }
}
