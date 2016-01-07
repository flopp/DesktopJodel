#include "imageWidget.h"
#include <QPainter>
#include <QDebug>

ImageWidget::ImageWidget(QWidget* parent) : QWidget(parent)
{
  QSizePolicy p;
  p.setHeightForWidth(true);
  setSizePolicy(p);
}


ImageWidget::~ImageWidget()
{
}


void ImageWidget::clear_image()
{
  set_image(QPixmap());
}


void ImageWidget::set_image(const QPixmap& image)
{
  _pixmap = image;

  if (width() > 0 && !_pixmap.isNull())
  {
    _scaled_pixmap = _pixmap.scaledToWidth(qMin(_pixmap.width(), width()));
  }
  else
  {
    _scaled_pixmap = QPixmap();
  }

  updateGeometry();
  update();
}


int ImageWidget::heightForWidth(int w) const
{
  if (_pixmap.isNull()) return -1;

  if (w >= _pixmap.width())
  {
    return _pixmap.height();
  }
  else
  {
    const double ratio = _pixmap.height() / (double)_pixmap.width();
    return w * ratio;
  }
}


void ImageWidget::resizeEvent(QResizeEvent* event)
{
  if (width() > 0 && !_pixmap.isNull() && _scaled_pixmap.width() != qMin(width(), _pixmap.width()))
  {
    _scaled_pixmap = _pixmap.scaledToWidth(qMin(width(), _pixmap.width()));
    update();
  }
}


void ImageWidget::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);

  if (!_scaled_pixmap.isNull())
  {
    QPainter p(this);
    p.drawPixmap((width()-_scaled_pixmap.width())/2, (height()-_scaled_pixmap.height())/2, _scaled_pixmap);
  }
}
