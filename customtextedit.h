#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H
#include <QMouseEvent>
#include <QPlainTextEdit>
#include <QDebug>
#include <QRegularExpression>
#include "edittermdialog.h"
#include "worddata.h"
#include "mainwindow.h"


class CustomTextEdit : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit CustomTextEdit(MainWindow *mainWin, QWidget *parent = nullptr) : QPlainTextEdit(parent), mainWindow(mainWin) {
        setMouseTracking(true); // Включаем отслеживание движения мыши
        //setCursorWidth(0);
    }

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override; // Обрабатываем клик
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    //void wordColorSelected(const QString &word, const QString &color); // Добавляем сигнал
    void wordDataHasChanged(const QString &word, const QString &color, WordData &currentWord);
private:
    MainWindow *mainWindow; // Указатель на MainWindow
    QString lastHoveredWord; // Последнее выделенное слово
};


#endif // CUSTOMTEXTEDIT_H
