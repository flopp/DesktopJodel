#include "imageFetcher.h"
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QUrl>

ImageFetcher::ImageFetcher(QObject* parent) : QObject(parent)
{
  _net = new QNetworkAccessManager(this);
  connect(_net, SIGNAL(finished(QNetworkReply*)), this, SLOT(handle_reply(QNetworkReply*)));
}


ImageFetcher::~ImageFetcher()
{
}


void ImageFetcher::fetch_image(const QUrl& url)
{
  const QPixmap cached_image = fetch_from_cache(url);
  if (cached_image.isNull())
  {
    _net->get(QNetworkRequest(url));
  }
  else
  {
    emit image_ready(url, cached_image);
  }
}


void ImageFetcher::handle_reply(QNetworkReply* reply)
{
  if (reply->error() == QNetworkReply::NoError)
  {
    const QByteArray data = reply->readAll();
    QPixmap image;
    if (image.loadFromData(data))
    {
      store_to_cache(reply->url(), image);
      emit image_ready(reply->url(), image);
    }
  }
  reply->deleteLater();
}


QPixmap ImageFetcher::fetch_from_cache(const QUrl& url)
{
  const QString file_name = url.fileName();
  if (!_temp_dir.isValid() || file_name.isEmpty()) return QPixmap();

  QDir dir(_temp_dir.path());
  if (!dir.exists(file_name)) return QPixmap();
  const QString file_path = dir.filePath(file_name);
  QPixmap image;
  image.load(file_path);
  return image;
}


void ImageFetcher::store_to_cache(const QUrl& url, const QPixmap& image)
{
  const QString file_name = url.fileName();
  if (!_temp_dir.isValid() || file_name.isEmpty()) return;

  const QDir dir(_temp_dir.path());
  const QString file_path = dir.filePath(file_name);
  image.save(file_path);
}
