#ifndef EDITTERMDIALOG_H
#define EDITTERMDIALOG_H

#include <QDialog>
#include <QColorDialog>
#include <QDesktopServices>
#include <qlabel.h>
#include <qplaintextedit.h>
#include "worddata.h"

namespace Ui {
class EditTermDialog;
}

class EditTermDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTermDialog(const QString &word, QWidget *parent = nullptr);
    explicit EditTermDialog(QHash<QString, WordData> &wordList, const QString &word, QWidget *parent = nullptr);
    ~EditTermDialog();

    QString selectedColor(); // Метод для получения выбранного цвета
    WordData getCurrentWord();

private slots:
    void on_buttonBox_accepted();

    void on_dict1Btn_clicked();

    void on_dict2Btn_clicked();

    void on_dict3Btn_clicked();

    void on_pushButton_clicked();

private:
    Ui::EditTermDialog *ui;
    QString color; // Хранит выбранный цвет
    WordData currentWord;
    QString currentWordString;
    QHash<QString, WordData> *wordListPointer;
    bool editable;

    QLabel *linkedTermsLabel;
    QLabel *meaningLabel;
    QLabel *pinyinLabel;
    QLabel *exampleLabel;

    QPlainTextEdit *meaningTextEdit;
    QPlainTextEdit *pinyinTextEdit;
    QPlainTextEdit *exampleTextEdit;

    void chooseColor();
};

#endif // EDITTERMDIALOG_H
