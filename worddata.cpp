#include "worddata.h"

WordData::WordData(const QString& translation, const QString& romanization,
                   const QString& sentence, const QString& status, const QStringList& linkedTerms)
    : translation(translation), romanization(romanization),
    sentence(sentence), status(status), linkedTerms(linkedTerms) {}

WordData::WordData() {

}

QString WordData::getWord() const
{
    return word;
}

QString WordData::getTranslation() const {
    return translation;
}

QString WordData::getRomanization() const {
    return romanization;
}

QString WordData::getSentence() const {
    return sentence;
}

QString WordData::getStatus() const {
    return status;
}

QString WordData::getTag() const
{
    return tag;
}

QStringList WordData::getLinkedTerms() const {
    return linkedTerms;
}

void WordData::setWord(const QString &newWord)
{
    word = newWord;
}

void WordData::setStatus(const QString& newStatus) {
    status = newStatus;
}

void WordData::setTag(const QString &newTag)
{
    tag = newTag;
}

void WordData::setRomanization(const QString& newRomanization) {
    romanization = newRomanization;
}

void WordData::setSentence(const QString& newSentence) {
    sentence = newSentence;
}

void WordData::setTranslation(const QString& newTranslation) {
    translation = newTranslation;
}

void WordData::setLinkedTerms(const QStringList& newLinkedTerms) {
    linkedTerms = newLinkedTerms;
}

void WordData::addLinkedTerm(const QString& term) {
    if (!linkedTerms.contains(term)) {
        linkedTerms.append(term);
    }
}
