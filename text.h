#ifndef TEXT_H
#define TEXT_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMap>

#include "dbmanager.h"
#include "worddata.h"
#include "cppjieba/Jieba.hpp"

class Text
{
public:
    Text();
    Text(bool readFromFile, const QString &newText, const DbManager *dbManager);

    void setTextStr(const QString& newText);
    void readFromFile(const QString& file);
    void setWordColors(const QHash<QString, WordData> &wordHashList);
    void parseTextWords();
    void formatText();
    void cutToWords();

    QString getTextStr();
    QStringList getTextWords();
    QMap<QString, QString> getWordColors();

    double getWordsPercent(const QString &status);
    double getTextReadability();

private:
    QString originalText;
    QString textStr;
    QString cutToWordsStr;
    QStringList wordsStrList;
    QMap<QString, QString> wordColors;
    QHash<QString, WordData> textWords;
    const QHash<QString, WordData> *dictionary;
    const DbManager *dbManager;
};

#endif // TEXT_H
