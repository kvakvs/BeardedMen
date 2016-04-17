#pragma once

#include <QString>
#include <QRegExp>

namespace bm { namespace util {

template <typename EachMatch>
QString re_replace(const char *str0, const char *regex_str,
                   EachMatch each_match)
{
    QString str(str0);
    QRegExp rx(regex_str);

    rx.setMinimal(true);

    int s = -1;
    while ((s = rx.indexIn(str, s+1)) >= 0) {
        QString repl = each_match(rx.cap(1));
        str.replace(s, rx.cap(0).length(), repl);
        s += repl.length();
    }

    return str;
}

}} // ns bm::util
