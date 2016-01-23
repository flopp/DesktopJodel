#include "emoji.h"
#include <QDir>
#include <QDebug>

QString Emoji::to_html(const QString& unicode_string) const
{
    load_codes();
    const QString escaped = unicode_string.toHtmlEscaped();

    QString result;
    for (int pos = 0; pos < escaped.size(); /**/)
    {
        bool found = false;
        for (int length = 8; length > 0; --length)
        {
            QString possible_code = escaped.mid(pos, length);
            auto it = _codes.find(possible_code);
            if (it != _codes.end())
            {
                result += QString("<img src=\":%1\">").arg(it.value());
                pos += length;
                found = true;
                break;
            }
        }
        if (!found)
        {
            if (escaped[pos] == '\n')
            {
              result += "<br />";
            }
            else
            {
              result += escaped[pos];
            }
            ++pos;
        }
    }
    return result;
}


void Emoji::load_codes() const
{
    if (!_codes.isEmpty()) return;

    for (const QString& file_name: QDir(":").entryList({"*.png"}))
    {
        const QString utf32_sequence = file_name.left(file_name.size() - 4);

        QString utf16_code;
        for (const QString& code: utf32_sequence.split("-"))
        {
            bool ok;
            unsigned int h = code.toInt(&ok, 16);
            
            if (h <= 0xFFFF)
            {
                utf16_code += QChar(h);
            }
            else
            {
                h = h - 0x10000;
                unsigned int lo = (h & 0x3FF) + 0xDC00;
                unsigned int hi = ((h >> 10) & 0x3FF) + 0xD800;
                utf16_code += QChar(hi);
                utf16_code += QChar(lo);
            }
        }
        
        _codes.insert(utf16_code, file_name);
    }
}
