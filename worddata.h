#ifndef WORDDATA_H
#define WORDDATA_H

#include <QString>
#include <QStringList>

class WordData {
public:
    WordData(const QString& translation, const QString& romanization,
             const QString& sentence, const QString& status, const QStringList& linkedTerms = {});
    WordData();

    QString getWord() const;
    QString getTag() const;
    QString getTranslation() const;
    QString getRomanization() const;
    QString getSentence() const;
    QString getStatus() const;
    QStringList getLinkedTerms() const;

    void setWord(const QString& newWord);
    void setTag(const QString& newTag);
    void setTranslation(const QString& newTranslation);
    void setRomanization(const QString& newRomanization);
    void setSentence(const QString& newSentence);
    void setStatus(const QString& newStatus);
    void setLinkedTerms(const QStringList& newLinkedTerms);

    void addLinkedTerm(const QString& term);

private:
    QString word;
    QString tag;
    QString translation;
    QString romanization;
    QString sentence;
    QString status;
    QStringList linkedTerms;
};

#endif // WORDDATA_H
