#include "client.h"
#include "post.h"
#include <QJsonArray>
#include <QJsonDocument>

QString Post::get_json_string() const
{
  QJsonDocument doc;
  doc.setObject(_json);
  return doc.toJson();
}


QList<Post> Post::get_children() const
{
  QList<Post> posts;

  for (const auto& obj : _json["children"].toArray())
  {
    Post post;
    post.set_json(obj.toObject());
    posts << post;
  }
  return posts;
}


void Post::vote_up()
{
  if (_client)
  {
    _client->vote_up(*this);
  }
}


void Post::vote_down()
{
  if (_client)
  {
    _client->vote_down(*this);
  }
}


void Post::refresh()
{
  if (_client)
  {
    try
    {
      const Post new_post = _client->fetch_post(get_post_id());
      if (new_post.get_post_id() == get_post_id())
      {
        set_json(new_post._json);
      }
    }
    catch(...)
    {}
  }
}


void Post::set_json(const QJsonObject& json)
{
  _json = json;
  _post_id = _json["post_id"].toString();
  _created_at = QDateTime::fromString(_json["created_at"].toString(), "yyyy-MM-ddThh:mm:ss.zZ");
}
