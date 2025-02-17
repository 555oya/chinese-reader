#ifndef WORDHIGHLIGHTER_H
#define WORDHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class WordHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit WordHighlighter(QTextDocument *parent = nullptr);
    void setWordColor(const QString &word, const QColor &color);
    void setWordColorRule(const QMap<QString, QString> &colorMap);
    void changeWordColorRule(const QMap<QString, QString> &colorMap);
    void setSpaceSize(int size);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;

    int spaceSize;


    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat newWordFormat;
    QTextCharFormat unknownWordFormat;
    QTextCharFormat nearlyUnknownWordFormat;
    QTextCharFormat learningWordFormat;
    QTextCharFormat nearlyKnownWordFormat;
    QTextCharFormat knownWordFormat;
    QTextCharFormat ignoredWordFormat;
    QTextCharFormat wellKnownWordFormat;

    QTextCharFormat checkStatus(const QString &val);
};

#endif // WORDHIGHLIGHTER_H
