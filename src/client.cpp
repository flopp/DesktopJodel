#include "client.h"
#include "imageFetcher.h"
#include "voteDirection.h"
#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

Client::Client(const QString& device_uid, const Location& home_location) :
  _device_uid(device_uid),
  _home_location(home_location),
  _location(home_location)
{
  _net = new QNetworkAccessManager(this);
  _image_fetcher = new ImageFetcher(this);
  connect(_image_fetcher, SIGNAL(image_ready(QUrl, QPixmap)), this, SIGNAL(image_ready(QUrl, QPixmap)));
}


void Client::set_headers(QNetworkRequest* request) const
{
  request->setRawHeader("User-Agent", _user_agent_string.toUtf8());
  request->setRawHeader("Connection", "keep-alive");
  request->setRawHeader("Accept-Encoding", "gzip");
  request->setRawHeader("Content-Type", "application/json; charset=UTF-8");
  request->setRawHeader("Authorization", (QString("Bearer %1").arg(_access_token)).toUtf8());
}


void Client::vote_up(Post& post)
{
  perform_vote(post, VoteDirection::UP);
}


void Client::vote_down(Post& post)
{
  perform_vote(post, VoteDirection::DOWN);
}


Post Client::fetch_post(const QString& post_id)
{
  const QJsonDocument doc = authenticated_request(MethodType::GET, "/posts/" + post_id);
  Post post;
  post.set_client(this);
  post.set_json(doc.object());
  return post;
}


void Client::perform_vote(Post& post, const VoteDirection& direction)
{
  if (post.get_voted() != VoteDirection::NONE || direction == VoteDirection::NONE || post.get_post_id().isEmpty()) return;
  try
  {
    authenticated_request(MethodType::PUT, QString("/posts/%1/%2vote").arg(post.get_post_id()).arg(toString(direction)));
  }
  catch (...)
  {
    return;
  }
}


QList<Post> Client::get_posts(const QString& url)
{
  const QJsonArray array = authenticated_request(MethodType::GET, url).object().value("posts").toArray();

  QList<Post> posts;
  for (const auto& obj : array)
  {
    Post p;
    p.set_client(this);
    p.set_json(obj.toObject());
    posts << p;
  }

  return posts;
}


QJsonDocument Client::authenticated_request(MethodType method, const QString& url, const QJsonDocument& payload)
{
  if (!is_authenticated())
  {
    throw QString("Unauthenticated");
  }

  if (_expiration_date.isValid() && _expiration_date < QDateTime::currentDateTime())
  {
    authenticate();
  }

//  qDebug() << "request: " << url << " " << payload.toJson() << "\n\n";
  QNetworkRequest request(QUrl(_base_api_url + url));
  set_headers(&request);

  QNetworkReply* reply = nullptr;
  switch (method)
  {
    case MethodType::GET:
      reply = _net->get(request);
      break;
    case MethodType::POST:
      reply = _net->post(request, payload.toJson());
      break;
    case MethodType::PUT:
      reply = _net->put(request, payload.toJson());
      break;
    case MethodType::DELETE:
      reply = _net->deleteResource(request);
      break;
  }

  QEventLoop loop;
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  const auto status = reply->error();
  if (status == QNetworkReply::NoError)
  {
    const QString strReply = (QString)reply->readAll();
    const QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
//    qDebug() << "Response:" << jsonResponse.toJson() << "\n\n";
    delete reply;
    return jsonResponse;
  }
  else
  {
    const QString strReply = (QString)reply->readAll();
    delete reply;
//    qDebug() << "status: " << status;
//    const QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
//    qDebug() << "Response:" << jsonResponse.toJson() << "\n\n";
    throw QString("request failed");
    return QJsonDocument();
  }
}
    

QString Client::randomDeviceUid()
{
  static QString chars = "abcdefghijklmnopqrstuvwxyz0123456789";
  const int randomStringLength = 64;

  QString randomString;
  for (int i=0; i<randomStringLength; ++i)
  {
    const int index = qrand() % chars.length();
    randomString += chars[index];
  }
  return randomString;
}



void Client::authenticate()
{
  QNetworkRequest request(QUrl(_base_api_url + "/users"));
  request.setRawHeader("User-Agent", _user_agent_string.toUtf8());
  request.setRawHeader("Accept-Encoding", "gzip");
  request.setRawHeader("Content-Type", "application/json; charset=UTF-8");

  QJsonObject loc_coordinates;
  loc_coordinates.insert("lat", _home_location.lat);
  loc_coordinates.insert("lng", _home_location.lng);
  QJsonObject location;
  location.insert("city", _home_location.city);
  location.insert("country", _home_location.country);
  location.insert("loc_accuracy", 19.0);
  location.insert("loc_coordinates", loc_coordinates);
  QJsonObject payload;
  payload.insert("client_id", _client_id);
  payload.insert("device_uid", _device_uid);
  payload.insert("location", location);

  QJsonDocument doc;
  doc.setObject(payload);
  QNetworkReply* reply = _net->post(request, doc.toJson());

  QEventLoop loop;
  connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  const auto status = reply->error();
  if (status == QNetworkReply::NoError)
  {
    const QString strReply = (QString)reply->readAll();
//    qDebug() << "Response:" << strReply;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

    const QJsonObject jsonObj = jsonResponse.object();

    _access_token = jsonObj.value("access_token").toString();
    _distinct_id = jsonObj.value("distinct_id").toString();
    const int expires_in = jsonObj.value("expires_in").toInt();
    if (expires_in > 0)
    {
      _expiration_date = QDateTime::currentDateTime().addSecs(expires_in);
    }
    _refresh_token = jsonObj.value("refresh_token").toString();

    delete reply;
  }
  else
  {
    delete reply;
//    qDebug() << "status: " << status;
    throw QString("authentication failed");
  }
}


int Client::get_karma()
{
  try
  {
    return authenticated_request(MethodType::GET, "/users/karma").object().value("karma").toInt(-1);
  }
  catch(...)
  {
    return -1;
  }
}


void Client::send_post(const QString& message, const QString& color)
{
  QJsonObject loc_coordinates;
  loc_coordinates.insert("lat", _location.lat);
  loc_coordinates.insert("lng", _location.lng);
  QJsonObject location;
  location.insert("city", _location.city);
  location.insert("country", _location.country);
  location.insert("loc_accuracy", 19.0);
  location.insert("loc_coordinates", loc_coordinates);
  location.insert("name", "41");
  QJsonObject payload;
  payload.insert("color", color);
  payload.insert("location", location);
  payload.insert("message", message);

  QJsonDocument doc;
  doc.setObject(payload);

  authenticated_request(MethodType::POST, "/posts/", doc);
}


void Client::send_reply(const QString& message, const QString& post_id, const QString& color)
{
  QJsonObject loc_coordinates;
  loc_coordinates.insert("lat", _location.lat);
  loc_coordinates.insert("lng", _location.lng);
  QJsonObject location;
  location.insert("city", _location.city);
  location.insert("country", _location.country);
  location.insert("loc_accuracy", 19.0);
  location.insert("loc_coordinates", loc_coordinates);
  location.insert("name", "41");
  QJsonObject payload;
  payload.insert("ancestor", post_id);
  payload.insert("color", color);
  payload.insert("location", location);
  payload.insert("message", message);

  QJsonDocument doc;
  doc.setObject(payload);

  authenticated_request(MethodType::POST, "/posts/", doc);
}


void Client::set_location(const Location& location)
{
  if (location != _location)
  {
    _location = location;

    QJsonObject loc_coordinates;
    loc_coordinates.insert("lat", _location.lat);
    loc_coordinates.insert("lng", _location.lng);
    QJsonObject json_location;
    json_location.insert("city", _location.city);
    json_location.insert("country", _location.country);
    json_location.insert("loc_accuracy", 19.0);
    json_location.insert("loc_coordinates", loc_coordinates);
    json_location.insert("name", "41");
    QJsonObject payload;
    payload.insert("location", json_location);

    QJsonDocument doc;
    doc.setObject(payload);

    try
    {
      authenticated_request(MethodType::PUT, "/users/place", doc);
    }
    catch(...)
    {
      qDebug() << "setting new location failed";
      return;
    }
  }
}


void Client::set_device_uid(const QString& device_uid)
{
  _device_uid = device_uid;
}


void Client::fetch_image(const QUrl& url)
{
  _image_fetcher->fetch_image(url);
}
