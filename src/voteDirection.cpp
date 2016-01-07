#include "voteDirection.h"


QString toString(const VoteDirection& dir)
{
  static QString up_string = "up";
  static QString down_string = "down";
  static QString none_string = "";

  switch (dir)
  {
    case VoteDirection::UP: return up_string;
    case VoteDirection::DOWN: return down_string;
    case VoteDirection::NONE: return none_string;
  }

  return none_string;
}


VoteDirection fromString(const QString& s)
{
  static QString up_string = "up";
  static QString down_string = "down";
  static QString none_string = "";

  if (s == up_string) { return VoteDirection::UP; }
  else if (s == down_string) { return VoteDirection::DOWN; }
  else return VoteDirection::NONE;
}
