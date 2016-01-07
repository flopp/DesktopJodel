#pragma once

#include "voteDirection.h"
#include <QWidget>
namespace Ui { class VoteWidget; }

class VoteWidget: public QWidget
{
    Q_OBJECT

  private:
    Ui::VoteWidget* _ui = nullptr;

  public:
    explicit VoteWidget(QWidget* parent = nullptr);
    virtual ~VoteWidget();

    void set_votes(int votes);
    void set_voted_direction(const VoteDirection& dir);

  signals:
    void up_vote();
    void down_vote();
};
