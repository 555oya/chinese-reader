#ifndef TEXT_H
#define TEXT_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMap>

#include "worddata.h"
#include "cppjieba/Jieba.hpp"

class Text
{
public:
    Text();
    Text(bool readFromFile, const QString &newText, const QHash<QString, WordData> &wordHashList);

    void setTextStr(const QString& newText);
    void readFromFile(const QString& file);
    void setWordColors(const QHash<QString, WordData> &wordHashList);
    void parseTextWords();
    void formatText();
    void cutToWords(const QHash<QString, WordData> &wordHashList);

    QString getTextStr();
    QStringList getTextWords();
    QMap<QString, QString> getWordColors();

    double getWordsPercent(const QString &status);

private:
    QString originalText;
    QString textStr;
    QString cutToWordsStr;
    QStringList wordsStrList;
    QMap<QString, QString> wordColors;
    QHash<QString, WordData> textWords;
};

#endif // TEXT_H
