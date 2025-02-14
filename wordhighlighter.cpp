#include "wordhighlighter.h"
#include <QFile>

WordHighlighter::WordHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {}

void WordHighlighter::setWordColor(const QString &word, const QColor &color) {
    wordColors[word] = color;
    rehighlight(); // Перерисовываем текст
}

void WordHighlighter::highlightBlock(const QString &text) {

    for (auto it = wordColors.begin(); it != wordColors.end(); ++it) {
        QRegularExpression regex(QString(R"((?<!\S)%1(?!\S))").arg(QRegularExpression::escape(it.key()))); // for regular spaces

        QRegularExpressionMatchIterator matches = regex.globalMatch(text);

        while (matches.hasNext()) {
            QRegularExpressionMatch match = matches.next();
            QTextCharFormat format;
            format.setBackground(it.value());
            setFormat(match.capturedStart(), match.capturedLength(), format);
        }
    }
}
