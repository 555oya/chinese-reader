#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>

#include "wordhighlighter.h"
#include "worddata.h"
#include "exitdialog.h"
#include "cppjieba/Jieba.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QHash<QString, WordData> *getWordHashList();

private slots:
    void on_pushButton_clicked();
    //void updateWordColor(const QString &word, const QString &color);
    void updateWordData(const QString &word, const QString &color, WordData &currentWord);

    void on_saveButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBoxHideSpaces_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::MainWindow *ui;
    WordHighlighter* highlighter;
    QHash<QString, WordData> wordHashList;

    void saveWordsToCSV(const QString& filePath);
    QStringList parseCSVLine(const QString& line);
    void loadWordsFromCSV(const QString& filePath);
    bool changesInWord;
    void closeEvent(QCloseEvent *event);
    void parseText(QString text);
    QString filepath;
};
#endif // MAINWINDOW_H
