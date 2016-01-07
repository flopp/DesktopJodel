#pragma once

#include <QString>

enum class VoteDirection
{
    NONE,
    UP,
    DOWN
};

QString toString(const VoteDirection& dir);
VoteDirection fromString(const QString& s);
