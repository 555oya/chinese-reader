#include "mainwindow.h"
#include "customtextedit.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "wordhighlighter.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CustomTextEdit *customTextEdit = new CustomTextEdit(this);
    ui->gridLayout->replaceWidget(ui->textEdit, customTextEdit);
    delete ui->textEdit; // Удаляем старый текстовый виджет
    ui->textEdit = customTextEdit;
    QFont font("SimSun", 20);
    ui->textEdit->setFont(font);

    highlighter = new WordHighlighter(customTextEdit->document());

    currentText = Text();

    settings = new QSettings(this);
    loadSettings();

    ui->checkBoxHideSpaces->setCheckState(Qt::Checked);

    connect(customTextEdit, &CustomTextEdit::wordDataHasChanged, this, &MainWindow::updateWordData);
    connect(customTextEdit, &CustomTextEdit::textHasChanged, this, &MainWindow::updateText);

    changesInWord = false;

    loadSettings();
    qDebug() << "defaultFolderSet " << defaultFolderSet;
    if (!defaultFolderSet) {
        QMessageBox::information(this, "Choose folder", "Select a directory for chinese-reader", QMessageBox::Ok);
        folderPath = QFileDialog::getExistingDirectory(this, tr("Choose directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        //проверка
        folderPath = folderPath + "/chinese-reader";
        defaultOpenFileFolderPath = folderPath + "/texts";
        termDictFolderPath = folderPath + "/terms";
        QDir dirText(defaultOpenFileFolderPath);
        if (!dirText.exists())
            dirText.mkpath(defaultOpenFileFolderPath);
        QDir dirTerm(termDictFolderPath);
        if (!dirTerm.exists())
            dirTerm.mkpath(termDictFolderPath);
    }
    termDictFilePath = folderPath + "/terms/term-dict.csv";
    defaultOpenFileFolderPath = folderPath + "/texts";
    termDictFolderPath = folderPath + "/terms";

    if(QFile::exists(termDictFilePath))
        loadWordsFromCSV(termDictFilePath);
    else
        qDebug() << "There is no term-sict.csv!! Path: " << termDictFilePath;

    ui->saveButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(true);

    int max_width = ui->nearlyUnknownWordsPercent->width();
    ui->label->setFixedWidth(max_width);
    ui->label_8->setFixedWidth(max_width);
    ui->label_2->setFixedWidth(max_width);
    ui->label_3->setFixedWidth(max_width);
    ui->label_4->setFixedWidth(max_width);
    ui->label_5->setFixedWidth(max_width);
    ui->label_6->setFixedWidth(max_width);
    ui->label_7->setFixedWidth(max_width);
}

MainWindow::~MainWindow()
{
    saveSettings();
    qDebug() << (settings->value("folderPath", "")).toString();
    delete ui;
    delete settings;
    delete highlighter;
}

QHash<QString, WordData> &MainWindow::getWordHashList()
{
    return wordHashList;
}

Text &MainWindow::getCurrentText()
{
    return currentText;
}

void MainWindow::getStatistics()
{
    ui->newWordsPercent->setText(QString::number(currentText.getWordsPercent("new"), 'f', 2) + "%");
    ui->unknownWordsPercent->setText(QString::number(currentText.getWordsPercent("unknown"), 'f', 2) + "%");
    ui->nearlyUnknownWordsPercent->setText(QString::number(currentText.getWordsPercent("nearlyUnknown"), 'f', 2) + "%");
    ui->learningWordsPercent->setText(QString::number(currentText.getWordsPercent("learning"), 'f', 2) + "%");
    ui->nearlyKnownWordsPercent->setText(QString::number(currentText.getWordsPercent("nearlyKnown"), 'f', 2) + "%");
    ui->knownWordsPercent->setText(QString::number(currentText.getWordsPercent("known"), 'f', 2) + "%");
    ui->ignoredWordsPercent->setText(QString::number(currentText.getWordsPercent("ignored"), 'f', 2) + "%");
    ui->wellKnownWordsPercent->setText(QString::number(currentText.getWordsPercent("wellKnown"), 'f', 2) + "%");
    ui->textReadability->setText(QString::number(currentText.getTextReadability(), 'f', 2) + "%");
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), defaultOpenFileFolderPath, tr("Text files (*.txt)"));
    qDebug() << "Text folder " << defaultOpenFileFolderPath;
    currentText = Text(true, fileName, wordHashList);

    highlighter->setWordColorRule(currentText.getWordColors());

    this->ui->textEdit->setPlainText(currentText.getTextStr());
    ui->saveButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    getStatistics();
    ui->groupBox->setHidden(false);
    ui->groupBox_2->setHidden(false);
}

void MainWindow::updateWordData(const QString &word, const QString &color, WordData &currentWord) {
    qDebug() << "Changing color for word:" << word << " to " << color;
    QMap<QString, QString> updatedWord;
    updatedWord.insert(word, color);
    highlighter->changeWordColorRule(updatedWord);
    highlighter->rehighlight();

    wordHashList.insert(word, currentWord);
    if(!changesInWord)
        changesInWord = true;
    currentText.setWordColors(wordHashList);
    getStatistics();
}

void MainWindow::updateText(const QString &newText)
{
    currentText = Text(false, newText, wordHashList);
    highlighter->setWordColorRule(currentText.getWordColors());
    highlighter->rehighlight();
    getStatistics();
}

void MainWindow::saveWordsToCSV(const QString& filePath) {
    qDebug() << "Saving to file path: " << filePath;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (auto wordItem = wordHashList.begin(); wordItem != wordHashList.end(); ++wordItem) {
            out << "\"" << wordItem.key() << "\"" << ","
                << "\"" << wordItem->getTranslation().replace("\n", "<br>") << "\"" << ","
                << "\"" << wordItem->getRomanization().replace("\n", "<br>") << "\"" << ","
                << "\"" << wordItem->getSentence().replace("\n", "<br>") << "\"" << ","
                << "\"" << wordItem->getStatus() << "\"" << ","
                << "\"" << wordItem->getLinkedTerms().join(";") << "\"" << "\n";
            qDebug() << "\"" << wordItem.key() << "\"" << ","
                     << "\"" << wordItem->getTranslation() << "\"" << ","
                     << "\"" << wordItem->getRomanization() << "\"" << ","
                     << "\"" << wordItem->getSentence() << "\"" << ","
                     << "\"" << wordItem->getStatus() << "\"" << ","
                     << "\"" << wordItem->getLinkedTerms().join(";") << "\"" << "\n";
        }
        file.close();
    }
}

QStringList MainWindow::parseCSVLine(const QString& line) {
    QStringList fields;
    QString currentField;
    bool inQuotes = false;

    for (int i = 0; i < line.length(); ++i) {
        QChar c = line[i];

        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.append(currentField.trimmed());
            currentField.clear();
        } else {
            currentField.append(c);
        }
    }

    if (!currentField.isEmpty()) {
        fields.append(currentField.trimmed());
    }

    return fields;
}

void MainWindow::loadWordsFromCSV(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file.errorString();
        return;
    }

    wordHashList.clear(); // Очистка перед загрузкой новых данных

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = parseCSVLine(line); // Парсим строку, учитывая кавычки

        if (fields.size() < 5) { // Минимально ожидаем 5 полей (слово + 4 параметра)
            qDebug() << "Skipping invalid line:" << line;
            continue;
        }

        QString word = fields[0];
        QString translation = fields[1].replace("<br>", "\n");
        QString romanization = fields[2].replace("<br>", "\n");
        QString sentence = fields[3].replace("<br>", "\n");
        QString status = fields[4];
        QStringList linkedTerms;

        if (fields.size() > 5) {
            linkedTerms = fields[5].split(";", Qt::SkipEmptyParts);
        }

        wordHashList.insert(word, WordData(translation, romanization, sentence, status, linkedTerms));
    }

    file.close();
    qDebug() << "Loaded words from CSV. Total:" << wordHashList.size();
}

void MainWindow::closeEvent(QCloseEvent *event) {

    if(changesInWord){
        ExitDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            saveWordsToCSV(termDictFilePath);
            QMessageBox::information(this, "Save", "All saved", QMessageBox::Ok);
            event->accept();
        }
        else {
            event->accept();
        }
    }
}

void MainWindow::saveSettings()
{
    settings->setValue("folderPath", folderPath);
    settings->setValue("defaultFolderSet", true);
}

void MainWindow::loadSettings()
{
    folderPath = settings->value("folderPath", "").toString();
    defaultFolderSet = settings->value("defaultFolderSet").toBool();

    qDebug() << "folder path: " << folderPath;
}

void MainWindow::on_saveButton_clicked()
{
    saveWordsToCSV(termDictFilePath);
    QMessageBox::information(this, "Save", "All saved", QMessageBox::Ok);
    changesInWord = false;
}

void MainWindow::on_pushButton_2_clicked()
{
    currentText.cutToWords(wordHashList);
    highlighter->setWordColorRule(currentText.getWordColors());

    this->ui->textEdit->setPlainText(currentText.getTextStr());

    getStatistics();
}

void MainWindow::on_checkBoxHideSpaces_checkStateChanged(const Qt::CheckState &arg1)
{
    if (arg1 == Qt::Checked)
        highlighter->setSpaceSize(1);
    else
        highlighter->setSpaceSize(20);
    highlighter->rehighlight();
}
