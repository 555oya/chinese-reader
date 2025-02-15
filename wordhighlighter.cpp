#include "wordhighlighter.h"
#include <QFile>

WordHighlighter::WordHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    newWordFormat.setBackground(QColor(170,170,255,125));
    unknownWordFormat.setBackground(QColor(255,83,169,125));
    nearlyUnknownWordFormat.setBackground(QColor(255,85,0,125));
    learningWordFormat.setBackground(QColor(255,255,0,125));
    nearlyKnownWordFormat.setBackground(QColor(170,255,0,125));
    knownWordFormat.setBackground(QColor(0,255,0,125));
    ignoredWordFormat.setBackground(QColor(140,140,140,125));
    wellKnownWordFormat.setBackground(QColor(170,170,255,0));

}

void WordHighlighter::setWordColor(const QString &word, const QColor &color) {
    //wordColors[word] = color;

    rehighlight(); // Перерисовываем текст
}

QTextCharFormat WordHighlighter::checkStatus(const QString &val) {
    if (val.compare("new") == 0) {
        return newWordFormat;
    }
    if (val.compare("unknown") == 0) {
        return unknownWordFormat;
    }
    if (val.compare("nearlyUnknown") == 0) {
        return nearlyUnknownWordFormat;
    }
    if (val.compare("learning") == 0) {
        return learningWordFormat;
    }
    if (val.compare("nearlyKnown") == 0) {
        return nearlyKnownWordFormat;
    }
    if (val.compare("known") == 0) {
        return knownWordFormat;
    }
    if (val.compare("ignored") == 0) {
        return ignoredWordFormat;
    }
    if (val.compare("wellKnown") == 0) {
        return wellKnownWordFormat;
    }
}

void WordHighlighter::changeWordColorRule(const QMap<QString, QString> &colorMap) {
    // for (auto it = colorMap.begin(); it != colorMap.end(); ++it) {
    //     qDebug() << "Looking for " << it.key() << " in rules " << it.value();
    //     for (auto rule = highlightingRules.begin(); rule != highlightingRules.end(); ++rule) {
    //         QRegularExpression regex(QString(R"((?<!\S)%1(?!\S))").arg(QRegularExpression::escape(it.key())));
    //         if (rule->pattern == regex){
    //             rule->format = checkStatus(it.value());
    //             qDebug() << "Changed word " << regex << " status to " <<  rule->format;
    //         }
    //     }
    // }
    // rehighlight();

    for (auto it = colorMap.begin(); it != colorMap.end(); ++it) {

        for (auto rule = highlightingRules.begin(); rule != highlightingRules.end(); ++rule) {
            QRegularExpression regex(QString(R"((?<!\S)%1(?!\S))").arg(QRegularExpression::escape(it.key())));

            if (rule->pattern == regex) {
                rule->format = checkStatus(it.value());
            }
        }
    }

    for (auto rule2 = highlightingRules.begin(); rule2 != highlightingRules.end(); ++rule2) {
        QString color;

        if (rule2->format == newWordFormat)
            color = "new";
        else if (rule2->format == unknownWordFormat)
            color = "unknown";
    }
}

void WordHighlighter::setWordColorRule(const QMap<QString, QString> &colorMap) {
    HighlightingRule rule;

    for (auto it = colorMap.begin(); it != colorMap.end(); ++it) {
        //qDebug() << "Set color rule " << it.value() << " for " << it.key();
        QRegularExpression regex(QString(R"((?<!\S)%1(?!\S))").arg(QRegularExpression::escape(it.key())));
        //QRegularExpression regex(QString(R"(\\b%1\\b)").arg(QRegularExpression::escape(it.key())));
        //qDebug() << regex;
        rule.pattern = regex;
        rule.format = checkStatus(it.value());
        highlightingRules.append(rule);
    }
    //rehighlight(); // Перерисовываем текст
}

void WordHighlighter::highlightBlock(const QString &text) {

    // for (auto it = wordColors.begin(); it != wordColors.end(); ++it) {
    //     QRegularExpression regex(QString(R"((?<!\S)%1(?!\S))").arg(QRegularExpression::escape(it.key()))); // for regular spaces

    //     QRegularExpressionMatchIterator matches = regex.globalMatch(text);

    //     while (matches.hasNext()) {
    //         QRegularExpressionMatch match = matches.next();
    //         QTextCharFormat format;
    //         format.setBackground(it.value());
    //         setFormat(match.capturedStart(), match.capturedLength(), format);
    //     }
    // }

    for (const HighlightingRule &rule : std::as_const(highlightingRules)) {
        //QRegularExpression regex(QString(R"((\\b)%1(\\b))").arg(QRegularExpression::escape("第一章"))); // for regular spaces
        //QRegularExpressionMatchIterator matchIterator = regex.globalMatch(text);

        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

}
