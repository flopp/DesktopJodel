#pragma once

#include "location.h"
#include "post.h"
#include <QDateTime>
#include <QJsonDocument>
#include <QList>
#include <QObject>
#include <QString>
class ImageFetcher;
class QNetworkAccessManager;
class QNetworkRequest;
class QPixmap;
class QUrl;

class Client: public QObject
{
    Q_OBJECT

  private:
    QString _device_uid;
    Location _home_location;
    Location _location;
    const QString _client_id = "81e8a76e-1e02-4d17-9ba0-8a7020261b26";
    const QString _user_agent_string = "Jodel/65000 Dalvik/2.1.0 (Linux; U; Android 5.0; SM-G900F Build/LRX21T)";
    const QString _base_api_url = "https://api.go-tellm.com/api/v2";

    QString _access_token;
    QString _distinct_id;
    QDateTime _expiration_date;
    QString _refresh_token;

    QNetworkAccessManager* _net = nullptr;
    ImageFetcher* _image_fetcher = nullptr;

  public:
    Client(const QString& device_uid, const Location& home_location);

    static QString randomDeviceUid();

    void authenticate();
    bool is_authenticated() const { return !_access_token.isEmpty(); }

    QList<Post> get_my_posts() { return get_posts("/posts/mine"); }
    QList<Post> get_my_replies() { return get_posts("/posts/mine/replies"); }
    QList<Post> get_my_votes() { return get_posts("/posts/mine/votes"); }
    QList<Post> get_my_top_posts() { return get_posts("/posts/mine/"); }

    QList<Post> get_local_posts() { return get_posts("/posts/location/"); }
    QList<Post> get_local_top_posts() { return get_posts("/posts/location/popular"); }
    QList<Post> get_local_discussed_posts() { return get_posts("/posts/location/discussed"); }

    QList<Post> get_home_posts() { return get_posts("/posts/"); }

    void vote_up(Post& post);
    void vote_down(Post& post);
    Post fetch_post(const QString& post_id);
    int get_karma();
//    void set_current_location();

    void send_post(const QString& message, const QString& color);
    void send_reply(const QString& message, const QString& post_id, const QString& color);

//    const double& get_lat() const { return _lat; }
//    const double& get_lng() const { return _lng; }
//    const QString& get_city() const { return _city; }
//    const QString& get_country_code() const { return _country_code; }
    const QString& get_device_uid() const { return _device_uid; }
    void set_location(const Location& location);
    void set_device_uid(const QString& device_uid);

    void fetch_image(const QUrl& url);

  signals:
    void image_ready(const QUrl& url, const QPixmap& image);

  private:
    void set_headers(QNetworkRequest* request) const;
    void perform_vote(Post& post, const VoteDirection& direction);
    QList<Post> get_posts(const QString& url);

    enum class MethodType { GET, POST, PUT, DELETE };
    QJsonDocument authenticated_request(MethodType method, const QString& url, const QJsonDocument& payload = QJsonDocument());
};
