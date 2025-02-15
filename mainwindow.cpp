#include "mainwindow.h"
#include "customtextedit.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "wordhighlighter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CustomTextEdit *customTextEdit = new CustomTextEdit(this);
    //ui->gridLayout->replaceWidget(ui->textEdit, customTextEdit);

    ui->gridLayout->replaceWidget(ui->textEdit, customTextEdit);
    delete ui->textEdit; // Удаляем старый текстовый виджет
    ui->textEdit = customTextEdit;
    //ui->textEdit->setFontPointSize(16);
    QFont font("SimSun", 20);
    ui->textEdit->setFont(font);
    //customTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    highlighter = new WordHighlighter(customTextEdit->document());
    connect(customTextEdit, &CustomTextEdit::wordColorSelected, this, &MainWindow::updateWordColor);
}

MainWindow::~MainWindow()
{
    delete ui;
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

    // QString modifiedText = text;
    // modifiedText.replace(" ", "\u200B"); // Заменяем пробелы
    // ui->textEdit->setPlainText(modifiedText);

    this->ui->textEdit->setPlainText(text);

    QString parsedStr = text;

    //удаление всех служебных символов
    QRegularExpression removeNonChineseRegex("[^一-龥a-zA-Z ]");
    QString cleanedText = parsedStr;
    cleanedText.remove(removeNonChineseRegex);

    // 2. Сжимаем несколько пробелов в один
    QRegularExpression multipleSpacesRegex("\\s+");
    cleanedText.replace(multipleSpacesRegex, " ");
    cleanedText = cleanedText.trimmed(); //удаляем пробелы в самом начале и в конце

    QStringList wordsStrList = cleanedText.split(" ");
    wordsStrList.removeDuplicates();

    QList<WordData*> wordList;
    QMap<QString, QString> wordColors;

    ui->textEdit->setPlainText(text);  // Устанавливаем текст один раз
    for (int i = 0; i < wordsStrList.size(); ++i) {
        //wordList.append(new WordData(wordsStrList[i]));

        wordColors.insert(wordsStrList[i], "new");

        // QColor chosenColor(170,170,255,125);
        // updateWordColor(wordsStrList[i], chosenColor);
        // qDebug() << i << " out of " << wordsStrList.size();
    }



    highlighter->setWordColorRule(wordColors);

    this->ui->textEdit->setPlainText(text);
    highlighter->rehighlight();
}


void MainWindow::on_pushButton_2_clicked()
{
    highlighter->setWordColor("碎星", Qt::red);
    highlighter->setWordColor("了", Qt::blue);
    highlighter->setWordColor("example", Qt::green);
    qDebug() << "Highlighting applied";
    highlighter->rehighlight();
}

void MainWindow::updateWordColor(const QString &word, const QString &color) {
    qDebug() << "Changing color for word:" << word << " to " << color;
    //highlighter->setWordColor(word, color);
    QMap<QString, QString> updatedWord;
    updatedWord.insert(word, color);
    highlighter->changeWordColorRule(updatedWord);
    highlighter->rehighlight();
}

