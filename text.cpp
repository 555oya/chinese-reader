#include "text.h"
#include <qregularexpression.h>

using namespace std;

Text::Text()
{

}

Text::Text(bool FromFile, const QString &newText, const QHash<QString, WordData> &wordHashList) {
    if (FromFile)
        readFromFile(newText);
    else
        setTextStr(newText);

    parseTextWords();
    setWordColors(wordHashList);
    formatText();
    dictionary = &wordHashList;
}

void Text::setTextStr(const QString &newText)
{
    textStr = newText;
}

void Text::readFromFile(const QString &newfile)
{
    QFile file = newfile;
    if (!file.exists()) { // Checks if exists
        qDebug("File doesn't exist!");
        return;
    }
    if(!file.open(QIODevice::ReadOnly)) { //checks if opened
        qDebug("Could not open the file");
        return;
    }

    QTextStream reading_stream(&file); //open stream for reading
    textStr = reading_stream.readAll();
    originalText = QString(textStr);
    originalText.remove(" ");
    file.close();
}

void Text::setWordColors(const QHash<QString, WordData> &wordHashList)
{
    for (int i = 0; i < wordsStrList.size(); ++i) {
        if(wordHashList.contains(wordsStrList[i]))
            wordColors.insert(wordsStrList[i], wordHashList.find(wordsStrList[i])->getStatus());
        else
            wordColors.insert(wordsStrList[i], "new");
    }
}

QString Text::getTextStr()
{
    return textStr;
}

QStringList Text::getTextWords()
{
    parseTextWords();
    return wordsStrList;
}

void Text::parseTextWords()
{
    QString parsedStr = textStr;

    //удаление всех служебных символов
    QRegularExpression removeNonChineseRegex("[^一-龥a-zA-Z ]");
    QString cleanedText = parsedStr;
    cleanedText.remove(removeNonChineseRegex);

    // 2. Сжимаем несколько пробелов в один
    QRegularExpression multipleSpacesRegex("\\s+");
    cleanedText.replace(multipleSpacesRegex, " ");
    cleanedText = cleanedText.trimmed(); //удаляем пробелы в самом начале и в конце

    wordsStrList = cleanedText.split(" ");

    wordsStrList.removeDuplicates();
}

void Text::formatText()
{
    QRegularExpression multipleSpacesRegex(" +");
    textStr.replace(multipleSpacesRegex, " ");
    textStr = textStr.trimmed(); //удаляем пробелы в самом начале и в конце
    //убираем пробелы до и после \n
    QRegularExpression regex("[ ]*(\n)[ ]*");
    textStr.replace(regex, "\\1");
    //убираем пробелы между знаками пунктуации
    QRegularExpression regex2("(?<![一-龥a-zA-Z]) (?![一-龥a-zA-Z])");
    textStr.remove(regex2);
}

void Text::cutToWords(const QHash<QString, WordData> &wordHashList)
{
    cppjieba::Jieba jieba(
        "dict/jieba.dict.utf8",
        "dict/hmm_model.utf8",
        "dict/user.dict.utf8",
        "dict/idf.utf8",
        "dict/stop_words.utf8"
        );
    vector<string> words;
    vector<cppjieba::Word> jiebawords;
    string s;
    string result;

    s = originalText.toStdString();
    jieba.Cut(s, words, true);

    result = limonp::Join(words.begin(), words.end(), " ");
    textStr = QString::fromStdString(result);
    parseTextWords();
    setWordColors(wordHashList);
    formatText();

    string text = textStr.toStdString();
}

QMap<QString, QString> Text::getWordColors()
{
    return wordColors;
}

double Text::getWordsPercent(const QString &status)
{
    QStringList allWords;
    QString parsedStr = textStr;

    //удаление всех служебных символов
    QRegularExpression removeNonChineseRegex("[^一-龥a-zA-Z ]");
    QString cleanedText = parsedStr;
    cleanedText.remove(removeNonChineseRegex);

    // 2. Сжимаем несколько пробелов в один
    QRegularExpression multipleSpacesRegex("\\s+");
    cleanedText.replace(multipleSpacesRegex, " ");
    cleanedText = cleanedText.trimmed(); //удаляем пробелы в самом начале и в конце

    allWords = cleanedText.split(" ");

    double count = 0;
    for (auto &word : allWords) {
        if (dictionary->contains(word)) {
            if(dictionary->find(word).value().getStatus().compare(status) == 0)
                count++;
        }
        else if (status.compare("new") == 0) {
            count++;
        }
    }
    double percent = count * 100.0 / (double)allWords.size();

    return percent;
}

double Text::getTextReadability()
{
    QStringList allWords;
    QString parsedStr = textStr;

    //удаление всех служебных символов
    QRegularExpression removeNonChineseRegex("[^一-龥a-zA-Z ]");
    QString cleanedText = parsedStr;
    cleanedText.remove(removeNonChineseRegex);

    // 2. Сжимаем несколько пробелов в один
    QRegularExpression multipleSpacesRegex("\\s+");
    cleanedText.replace(multipleSpacesRegex, " ");
    cleanedText = cleanedText.trimmed(); //удаляем пробелы в самом начале и в конце

    allWords = cleanedText.split(" ");

    QList<QPair<double, double>> wordFreq;

    for (auto &word : wordsStrList) {
        double count = allWords.count(word);
        if (dictionary->contains(word)) {
            if(dictionary->find(word).value().getStatus().compare("wellKnown") == 0)
                wordFreq.append(QPair<double, double>(count, 1));
            if(dictionary->find(word).value().getStatus().compare("known") == 0)
                wordFreq.append(QPair<double, double>(count, 1));
            if(dictionary->find(word).value().getStatus().compare("nearlyKnown") == 0)
                wordFreq.append(QPair<double, double>(count, 0.75));
            if(dictionary->find(word).value().getStatus().compare("learning") == 0)
                wordFreq.append(QPair<double, double>(count, 0.5));
        }
    }
    double sum = 0;
    for (auto &word_pair : wordFreq)
        sum += (word_pair.first * word_pair.second);

    double percent = sum * 100.0 / (double)allWords.size();

    return percent;
}



