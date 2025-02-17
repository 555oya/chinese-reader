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

    ui->checkBoxHideSpaces->setCheckState(Qt::Checked);

    connect(customTextEdit, &CustomTextEdit::wordDataHasChanged, this, &MainWindow::updateWordData);
    changesInWord = false;

    filepath = "C:/Users/anast/OneDrive/Documents/chinese-reader/terms/term-dict.csv";

    loadWordsFromCSV(filepath);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QHash<QString, WordData> *MainWindow::getWordHashList()
{
    return &wordHashList;
}

void MainWindow::parseText(QString text) {
    QString parsedStr = text;

    text.replace(QChar(0x200B), ' ');
    //удаление всех служебных символов
    QRegularExpression removeNonChineseRegex("[^一-龥a-zA-Z ]");
    // QRegularExpression removeNonChineseRegex("[^一-龥a-zA-Z \u200B]");
    QString cleanedText = parsedStr;
    cleanedText.remove(removeNonChineseRegex);

    // 2. Сжимаем несколько пробелов в один
    QRegularExpression multipleSpacesRegex("\\s+");
    cleanedText.replace(multipleSpacesRegex, " ");
    cleanedText = cleanedText.trimmed(); //удаляем пробелы в самом начале и в конце

    QStringList wordsStrList = cleanedText.split(" ");

    wordsStrList.removeDuplicates();

    QMap<QString, QString> wordColors;

    for (int i = 0; i < wordsStrList.size(); ++i) {
        if(wordHashList.contains(wordsStrList[i]))
            wordColors.insert(wordsStrList[i], wordHashList.find(wordsStrList[i])->getStatus());
        else
            wordColors.insert(wordsStrList[i], "new");
    }
    highlighter->setWordColorRule(wordColors);
}

void MainWindow::on_pushButton_clicked()
{
    //Open file with explorer
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/Users/anast/OneDrive/Documents/chinese-reader/texts", tr("Text files (*.txt)"));
    QFile file(fileName);
    if (!file.exists()) { // Checks if exists
        qDebug("File doesn't exist!");
        return;
    }
    if(!file.open(QIODevice::ReadOnly)) { //checks if opened
        qDebug("Could not open the file");
    }

    QTextStream reading_stream(&file); //open stream for reading
    QString text = reading_stream.readAll();
    file.close();

    parseText(text);

    this->ui->textEdit->setPlainText(text);

    highlighter->rehighlight();
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
}

void MainWindow::saveWordsToCSV(const QString& filePath) {
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
        saveWordsToCSV(filepath);
        QMessageBox::information(this, "Save", "All saved", QMessageBox::Ok);
        event->accept();
    }
    else {
        event->accept();
    }
    }
}

void MainWindow::on_saveButton_clicked()
{
    saveWordsToCSV(filepath);
    QMessageBox::information(this, "Save", "All saved", QMessageBox::Ok);
    changesInWord = false;
}


void MainWindow::on_pushButton_2_clicked()
{
    QString text = ui->textEdit->toPlainText();
    cppjieba::Jieba jieba;
    vector<string> words;
    vector<cppjieba::Word> jiebawords;
    string s;
    string result;

    s = text.toStdString();
    jieba.Cut(s, words, true);

    result = limonp::Join(words.begin(), words.end(), " ");
    QString textQString = QString::fromStdString(result);



    ui->textEdit->setPlainText(textQString);

    parseText(textQString);
    highlighter->rehighlight();
}


void MainWindow::on_checkBoxHideSpaces_checkStateChanged(const Qt::CheckState &arg1)
{
    if (arg1 == Qt::Checked)
        highlighter->setSpaceSize(1);
    else
        highlighter->setSpaceSize(20);
    highlighter->rehighlight();
}

