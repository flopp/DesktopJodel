#pragma once

#include <QWidget>
#include <QPixmap>

class ImageWidget: public QWidget
{
    Q_OBJECT

  public:
    ImageWidget(QWidget* parent = nullptr);
    virtual ~ImageWidget();

    void clear_image();
    void set_image(const QPixmap& image);

    virtual int heightForWidth(int w) const override;

  protected:
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent* event);

  private:
    QPixmap _pixmap;
    QPixmap _scaled_pixmap;
};
