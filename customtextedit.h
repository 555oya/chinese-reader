#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H
#include <QMouseEvent>
#include <QPlainTextEdit>
#include <QDebug>
#include <QRegularExpression>
#include "edittermdialog.h"


class CustomTextEdit : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit CustomTextEdit(QWidget *parent = nullptr) : QPlainTextEdit(parent) {}

protected:
    void mousePressEvent(QMouseEvent *event) override; // Обрабатываем клик

signals:
    void wordColorSelected(const QString &word, const QString &color); // Добавляем сигнал

};


#endif // CUSTOMTEXTEDIT_H
