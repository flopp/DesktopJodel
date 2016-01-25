#pragma once

#include <QString>
#include <QHash>

class Emoji
{
  public:
    static const Emoji& instance() {
      static Emoji e;
      return e;
    }

    QString to_html(const QString& unicodeString) const;

  private:
    Emoji() {}
    Emoji(const Emoji&) = delete;
    Emoji& operator=(const Emoji&) = delete;

    void load_codes() const;
    mutable QHash<QString, QString> _codes;
};

inline const Emoji& emoji() { return Emoji::instance(); }



