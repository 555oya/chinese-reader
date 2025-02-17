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

    spaceSize = 20;
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
    for (auto it = colorMap.begin(); it != colorMap.end(); ++it) {

        for (auto rule = highlightingRules.begin(); rule != highlightingRules.end(); ++rule) {
            QRegularExpression regex(QString(R"((?<!\S)%1(?!\S))").arg(QRegularExpression::escape(it.key())));
            // QRegularExpression regex(QString(R"((?:^|[\s\x{200B}])%1(?=[\s\x{200B}]|$))").arg(QRegularExpression::escape(it.key())),
            //                          QRegularExpression::UseUnicodePropertiesOption);
            if (rule->pattern == regex) {
                rule->format = checkStatus(it.value());
            }
        }
    }
}

void WordHighlighter::setSpaceSize(int size)
{
    spaceSize = size;
}

void WordHighlighter::setWordColorRule(const QMap<QString, QString> &colorMap) {
    HighlightingRule rule;

    for (auto it = colorMap.begin(); it != colorMap.end(); ++it) {
        QRegularExpression regex(QString(R"((?<!\S)%1(?!\S))").arg(QRegularExpression::escape(it.key())));
        // QRegularExpression regex(QString(R"((?:^|[\s\x{200B}])%1(?=[\s\x{200B}]|$))").arg(QRegularExpression::escape(it.key())),
        //                          QRegularExpression::UseUnicodePropertiesOption);
        rule.pattern = regex;
        rule.format = checkStatus(it.value());
        highlightingRules.append(rule);
    }
}

void WordHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : std::as_const(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    QTextCharFormat fmt;
    fmt.setFontPointSize(spaceSize);

    QRegularExpression whitespaceRegex(" ");
    QRegularExpressionMatchIterator wsMatchIterator = whitespaceRegex.globalMatch(text);

    while (wsMatchIterator.hasNext()) {
        QRegularExpressionMatch match = wsMatchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), fmt);
    }
}
