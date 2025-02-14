#ifndef WORDDATA_H
#define WORDDATA_H

#include <QString>
#include <QStringList>

class WordData {
public:
    WordData(const QString& word, const QString& translation, const QString& romanization,
             const QString& sentence, const QString& status, const QStringList& linkedTerms = {});
    WordData(const QString& word);

    QString getWord() const;
    QString getTranslation() const;
    QString getRomanization() const;
    QString getSentence() const;
    QString getStatus() const;
    QStringList getLinkedTerms() const;

    void setWord(const QString& newWord);
    void setTranslation(const QString& newTranslation);
    void setRomanization(const QString& newRomanization);
    void setSentence(const QString& newSentence);
    void setStatus(const QString& newStatus);
    void setLinkedTerms(const QStringList& newLinkedTerms);
    void addLinkedTerm(const QString& term);

private:
    QString word;
    QString translation;
    QString romanization;
    QString sentence;
    QString status;
    QStringList linkedTerms;
};

#endif // WORDDATA_H
