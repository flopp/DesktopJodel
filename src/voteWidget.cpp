#include "voteWidget.h"
#include "ui_voteWidget.h"


VoteWidget::VoteWidget(QWidget* parent) :
  QWidget(parent),
  _ui(new Ui::VoteWidget)
{
  _ui->setupUi(this);

  connect(_ui->upButton, SIGNAL(clicked()), this, SIGNAL(up_vote()));
  connect(_ui->downButton, SIGNAL(clicked()), this, SIGNAL(down_vote()));
}


VoteWidget::~VoteWidget()
{
  delete _ui;
}


void VoteWidget::set_votes(int votes)
{
  _ui->label->setText(QString::number(votes));
}


void VoteWidget::set_voted_direction(const VoteDirection& dir)
{
  static const QString triangle_up_filled = QString::fromUtf8("\u25B2");
  static const QString triangle_up_empty = QString::fromUtf8("\u25B3");
  static const QString triangle_down_filled = QString::fromUtf8("\u25BC");
  static const QString triangle_down_empty = QString::fromUtf8("\u25BD");

  switch (dir)
  {
    case VoteDirection::NONE:
      _ui->upButton->setEnabled(true);
      _ui->downButton->setEnabled(true);
      _ui->upButton->setText(triangle_up_filled);
      _ui->downButton->setText(triangle_down_filled);
      break;
    case VoteDirection::UP:
      _ui->upButton->setEnabled(false);
      _ui->downButton->setEnabled(false);
      _ui->upButton->setText(triangle_up_filled);
      _ui->downButton->setText(triangle_down_empty);
      break;
    case VoteDirection::DOWN:
      _ui->upButton->setEnabled(false);
      _ui->downButton->setEnabled(false);
      _ui->upButton->setText(triangle_up_empty);
      _ui->downButton->setText(triangle_down_filled);
      break;
  }
}
