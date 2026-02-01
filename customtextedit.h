#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QMouseEvent>
#include <QPlainTextEdit>
#include <QDebug>
#include <QRegularExpression>
#include <QMenu>
#include <QDesktopServices>

#include "edittermdialog.h"
#include "worddata.h"
#include "mainwindow.h"


class CustomTextEdit : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit CustomTextEdit(DbManager *dbManager, QStatusBar *statusBar, QWidget *parent = nullptr) : QPlainTextEdit(parent) {
        setMouseTracking(true); // Включаем отслеживание движения мыши
        setReadOnly(true);
        QFont newfont("SimSun", 20);
        font = newfont;
        setFont(font);
        this->dbManager = dbManager;
        this->statusBar = statusBar;
    }

private slots:
    void insertSpace();
    void joinWords();
    void useGoogle();
    void useBaidu();

    void addNDict();
    void addNRDict();
    void addNSDict();
    void addNTDict();
    void addNZDict();

    void addVDict();
    void addVNDict();
    void addVDDict();

    void addADict();
    void addDDict();
    void addIDict();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override; // Обрабатываем клик
    void mouseMoveEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

signals:
    void wordDataHasChanged(const QString &word, const QString &color, WordData &currentWord);
    void textHasChanged(const QString &newText);
private:
    MainWindow *mainWindow; // Указатель на MainWindow
    QString lastHoveredWord; // Последнее выделенное слово
    QContextMenuEvent *rmbEvent;

    DbManager *dbManager;
    QStatusBar *statusBar;

    QFont font;
};


#endif // CUSTOMTEXTEDIT_H
