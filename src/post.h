#pragma once

#include <QJsonObject>
#include <QColor>
#include <QDateTime>
#include "voteDirection.h"
class Client;

class Post
{
  private:
    QJsonObject _json;
    Client* _client = nullptr;

    QString _post_id;
    QDateTime _created_at;

  public:
    void set_client(Client* client) { _client = client; }
    Client* client() const { return _client; }

    void set_json(const QJsonObject& json);
    QString get_json_string() const;

    const QString& get_post_id() const { return _post_id; }
    QString get_message() const { return _json["message"].toString(); }
    QString get_color_string() const { return _json["color"].toString("9EC41C"); }
    QColor get_color() const { return QColor("#" + get_color_string()); }
    QList<Post> get_children() const;
    int get_child_count() const  { return _json["child_count"].toInt(0); }
    int get_votes() const { return _json["vote_count"].toInt(0); }
    // "", "up", "down"
    VoteDirection get_voted() const { return fromString(_json["voted"].toString()); }
    const QDateTime& get_created_at() const { return _created_at; }
    QDateTime get_updated_at() const { return QDateTime::fromString(_json["updated_at"].toString(), "yyyy-MM-ddThh:mm:ss.zZ"); }
    QString get_city() const { return _json["location"].toObject()["name"].toString(); }
    QString get_lat() const { return _json["location"].toObject()["loc_coordinates"].toObject()["lat"].toString(); }
    QString get_lng() const { return _json["location"].toObject()["loc_coordinates"].toObject()["lng"].toString(); }
    QString get_image_url() const { return _json["image_url"].toString(); }
    QString get_thumbnail_url() const { return _json["thumbnail_url"].toString(); }
    bool is_created_by_op() const { return _json["parent_creator"].toInt(0) == 1; }

    void vote_up();
    void vote_down();
    void refresh();
};

struct OrderPostsByDate
{
    bool operator()(const Post& left, const Post& right) const
    {
      if (left.get_created_at() != right.get_created_at()) return left.get_created_at() > right.get_created_at();
      return left.get_post_id() < right.get_post_id();
    }
};
