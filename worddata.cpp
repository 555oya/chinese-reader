#include "worddata.h"

WordData::WordData(const QString& translation, const QString& romanization,
                   const QString& sentence, const QString& status)
    : translation(translation), romanization(romanization),
    sentence(sentence), status(status) {}

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

void WordData::setWord(const QString &newWord)
{
    word = newWord;
}

void WordData::setStatus(const QString& newStatus) {
    status = newStatus;
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
