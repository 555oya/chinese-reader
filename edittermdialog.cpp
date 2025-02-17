#include "edittermdialog.h"
#include "ui_edittermdialog.h"

EditTermDialog::EditTermDialog(const QString &word, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditTermDialog)
{
    ui->setupUi(this);
    ui->wordLabel->setText(word); // Устанавливаем слово в label
    ui->radioBtnUnknown->setChecked(true);
}

EditTermDialog::EditTermDialog(QHash<QString, WordData> &wordList, const QString &word, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditTermDialog)
{
    ui->setupUi(this);
    wordListPointer = &wordList;
    currentWordString = word;
    ui->wordLabel->setText(word); // Устанавливаем слово в label
    ui->radioBtnUnknown->setChecked(true);
    if (wordListPointer->contains(word)){
        ui->meaningTextEdit->setPlainText(wordList.value(word).getTranslation());
        ui->pinyinTextEdit->setPlainText(wordList.value(word).getRomanization());
        ui->exampleTextEdit->setPlainText(wordList.value(word).getSentence());
        if(wordList.value(word).getStatus().compare("unknown") == 0)
            ui->radioBtnUnknown->setChecked(true);
        else if(wordList.value(word).getStatus().compare("nearlyUnknown") == 0)
            ui->radioBtnBetter->setChecked(true);
        else if(wordList.value(word).getStatus().compare("learning") == 0)
            ui->radioBtnLearning->setChecked(true);
        else if(wordList.value(word).getStatus().compare("nearlyKnown") == 0)
            ui->radioBtnNearlyKnown->setChecked(true);
        else if(wordList.value(word).getStatus().compare("known") == 0)
            ui->radioBtnKnown->setChecked(true);
        else if(wordList.value(word).getStatus().compare("ignored") == 0)
            ui->radioBtnIgnore->setChecked(true);
        else if(wordList.value(word).getStatus().compare("wellKnown") == 0)
            ui->radioBtnWellKnown->setChecked(true);
    }
}

EditTermDialog::~EditTermDialog()
{
    delete ui;
}

void EditTermDialog::chooseColor() {
    if(ui->radioBtnUnknown->isChecked()) {
        color = "unknown";
    }
    if(ui->radioBtnBetter->isChecked()) {
        color = "nearlyUnknown";
    }
    if(ui->radioBtnLearning->isChecked()) {
        color = "learning";
    }
    if(ui->radioBtnNearlyKnown->isChecked()) {
        color = "nearlyKnown";
    }
    if(ui->radioBtnKnown->isChecked()) {
        color = "known";
    }
    if(ui->radioBtnIgnore->isChecked()) {
        color = "ignored";
    }
    if(ui->radioBtnWellKnown->isChecked()) {
        color = "wellKnown";
    }
}

QString EditTermDialog::selectedColor() {
    chooseColor();
    return color;
}

WordData EditTermDialog::getCurrentWord() {
    return currentWord;
}

void EditTermDialog::on_buttonBox_accepted()
{
    currentWord.setRomanization(ui->pinyinTextEdit->toPlainText());
    currentWord.setTranslation(ui->meaningTextEdit->toPlainText());
    currentWord.setSentence(ui->exampleTextEdit->toPlainText());
    currentWord.setStatus(selectedColor());
    qDebug() << "Put info" << selectedColor();
    wordListPointer->insert(currentWordString, currentWord);

}


void EditTermDialog::on_dict1Btn_clicked()
{
    QString urlString = "https://bkrs.info/slovo.php?ch=" + currentWordString;
    QDesktopServices::openUrl(QUrl(urlString));
}


void EditTermDialog::on_dict2Btn_clicked()
{
    QString urlString = "https://translate.google.com/?ie=UTF-8&sl=zh&tl=ru&text=" + currentWordString;
    QDesktopServices::openUrl(QUrl(urlString));
}


void EditTermDialog::on_dict3Btn_clicked()
{
    QString urlString = "https://fanyi.baidu.com/mtpe-individual/multimodal?query=" + currentWordString + "&lang=zh2ru";
    QDesktopServices::openUrl(QUrl(urlString));
}

