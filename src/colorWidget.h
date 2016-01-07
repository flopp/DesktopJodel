#pragma once

#include <QWidget>
#include <QList>
#include <QString>
class QPushButton;
class QSignalMapper;
class QHBoxLayout;

class ColorWidget: public QWidget
{
    Q_OBJECT

  private:
    QList<QString> _colors;
    QList<QPushButton*> _buttons;
    QSignalMapper* _mapper = nullptr;
    int _selected_color_index = -1;
    QHBoxLayout* _layout = nullptr;

  public:
    ColorWidget(QWidget* parent = nullptr);
    virtual ~ColorWidget();

    void set_colors(const QList<QString>& colors);
    QString selected_color() const;

  private slots:
    void button_pressed(int index);
};
