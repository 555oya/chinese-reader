#ifndef TEXT_H
#define TEXT_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QCoreApplication>

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

private:
    QString textStr;
    QString cutToWordsStr;
    QStringList wordsStrList;
    QMap<QString, QString> wordColors;
};

#endif // TEXT_H
