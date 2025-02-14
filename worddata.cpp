#include "worddata.h"

WordData::WordData(const QString& word, const QString& translation, const QString& romanization,
                   const QString& sentence, const QString& status, const QStringList& linkedTerms)
    : word(word), translation(translation), romanization(romanization),
    sentence(sentence), status(status), linkedTerms(linkedTerms) {}

WordData::WordData(const QString& word)
    : word(word) {
    status = "New";
}

QString WordData::getWord() const {
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

QStringList WordData::getLinkedTerms() const {
    return linkedTerms;
}

void WordData::setStatus(const QString& newStatus) {
    status = newStatus;
}

void WordData::setLinkedTerms(const QStringList& newLinkedTerms) {
    linkedTerms = newLinkedTerms;
}

void WordData::addLinkedTerm(const QString& term) {
    if (!linkedTerms.contains(term)) {
        linkedTerms.append(term);
    }
}
