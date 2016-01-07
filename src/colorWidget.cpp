/************************************************
 *                                              *
 *    Copyright (c)                             *
 *    ATRiCS GmbH                               *
 *    Am Flughafen 7                            *
 *    79108 Freiburg                            *
 *    Germany                                   *
 *                                              *
 *    Author:   Florian Pigorsch                *
 *                                              *
 *    Created:  05.01.2016                     *
 *                                              *
 ************************************************/

#include "colorWidget.h"
#include <QSignalMapper>
#include <QPushButton>
#include <QHBoxLayout>

ColorWidget::ColorWidget(QWidget* parent) : QWidget(parent)
{
  _layout = new QHBoxLayout(this);
  _mapper = new QSignalMapper(this);
  connect(_mapper, SIGNAL(mapped(int)), this, SLOT(button_pressed(int)));
}


ColorWidget::~ColorWidget()
{
}


void ColorWidget::set_colors(const QList<QString>& colors)
{
  qDeleteAll(_buttons);
  _buttons.clear();

  _colors = colors;
  for (int i = 0; i < _colors.size(); ++i)
  {
    QPushButton* b = new QPushButton(this);
    b->setFixedWidth(b->height());
    b->setStyleSheet(QString("background-color: #%1; color: #ffffff;").arg(_colors[i]));
    _buttons << b;
    _layout->addWidget(b);
    _mapper->setMapping(b, i);
    connect(b, SIGNAL(clicked()), _mapper, SLOT(map()));
  }

  if (_colors.isEmpty())
  {
    button_pressed(-1);
  }
  else
  {
    button_pressed(0);
  }
}


QString ColorWidget::selected_color() const
{
  if (_selected_color_index < 0) return QString();
  else return _colors[_selected_color_index];
}


void ColorWidget::button_pressed(int index)
{
  static const QString big_x = QString::fromUtf8("\u2715");

  _selected_color_index = index;
  for (auto b: _buttons)
  {
    b->setText(QString());
  }
  if (_selected_color_index >= 0)
  {
    _buttons[_selected_color_index]->setText(big_x);
  }
}
