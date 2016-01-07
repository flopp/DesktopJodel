#pragma once

#include <QObject>
#include <QString>
#include <QTemporaryDir>
class QPixmap;
class QNetworkAccessManager;
class QNetworkReply;
class QUrl;

class ImageFetcher: public QObject
{
    Q_OBJECT

  public:
    ImageFetcher(QObject* parent = nullptr);
    virtual ~ImageFetcher();

    void fetch_image(const QUrl& url);

  signals:
    void image_ready(const QUrl& url, const QPixmap& image);

  private slots:
    void handle_reply(QNetworkReply* reply);

  private:
    QPixmap fetch_from_cache(const QUrl& url);
    void store_to_cache(const QUrl& url, const QPixmap& image);

  private:
    QNetworkAccessManager* _net = nullptr;
    QTemporaryDir _temp_dir;
};
