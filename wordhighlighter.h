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

protected:
    void highlightBlock(const QString &text) override;

private:
    QMap<QString, QColor> wordColors;
};

#endif // WORDHIGHLIGHTER_H
