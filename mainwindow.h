#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStandardPaths>
#include <QSettings>

#include "dbmanager.h"
#include "wordhighlighter.h"
#include "worddata.h"
#include "exitdialog.h"
#include "dictionarydialog.h"
#include "cppjieba/Jieba.hpp"
#include "text.h"

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

    void parseText(QString text);
    void getStatistics();

private slots:
    void on_pushButton_clicked();
    void updateWordData(const QString &word, const QString &color, WordData &currentWord);
    void updateText(const QString &word);
    void on_pushButton_2_clicked();
    void on_checkBoxHideSpaces_checkStateChanged(const Qt::CheckState &arg1);

    void on_dictionaryButton_clicked();

private:
    QSettings * settings;
    bool defaultFolderSet;

    Ui::MainWindow *ui;
    WordHighlighter* highlighter;
    cppjieba::Jieba *jieba;
    DbManager *dbManager;  // Добавляем указатель на менеджер базы данных

    void saveWordsToCSV(const QString& filePath);
    QStringList parseCSVLine(const QString& line);
    void loadWordsFromCSV(const QString& filePath);
    bool changesInWord;
    void closeEvent(QCloseEvent *event);
    void saveSettings();
    void loadSettings();

    Text currentText;

    QString folderPath;
    QString termDictFilePath;
    QString termDictFolderPath;
    QString defaultOpenFileFolderPath;
    QString databasePath;  // Путь к базе данных
};
#endif // MAINWINDOW_H
