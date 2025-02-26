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
    editable = true;

    linkedTermsLabel = new QLabel();
    ui->formLayout_3->replaceWidget(ui->linkedTermsTextEdit, linkedTermsLabel);
    delete ui->linkedTermsTextEdit; // Удаляем старый текстовый виджет

    if (wordListPointer->contains(word)){
        meaningLabel = new QLabel();
        ui->formLayout_3->replaceWidget(ui->meaningTextEdit, meaningLabel);
        delete ui->meaningTextEdit; // Удаляем старый текстовый виджет

        pinyinLabel = new QLabel(this);
        ui->formLayout_3->replaceWidget(ui->pinyinTextEdit, pinyinLabel);
        delete ui->pinyinTextEdit; // Удаляем старый текстовый виджет

        exampleLabel = new QLabel(this);
        ui->formLayout_3->replaceWidget(ui->exampleTextEdit, exampleLabel);
        delete ui->exampleTextEdit; // Удаляем старый текстовый виджет

        meaningLabel->setWordWrap(true);
        pinyinLabel->setWordWrap(true);
        exampleLabel->setWordWrap(true);

        meaningLabel->setText(wordList.value(word).getTranslation());
        pinyinLabel->setText(wordList.value(word).getRomanization());
        exampleLabel->setText(wordList.value(word).getSentence());

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

        editable = false;
    }
    else {
        meaningTextEdit = new QPlainTextEdit();
        ui->formLayout_3->replaceWidget(ui->meaningTextEdit, meaningTextEdit);
        delete ui->meaningTextEdit; // Удаляем старый текстовый виджет

        pinyinTextEdit = new QPlainTextEdit();
        ui->formLayout_3->replaceWidget(ui->pinyinTextEdit, pinyinTextEdit);
        delete ui->pinyinTextEdit; // Удаляем старый текстовый виджет

        exampleTextEdit = new QPlainTextEdit();
        ui->formLayout_3->replaceWidget(ui->exampleTextEdit, exampleTextEdit);
        delete ui->exampleTextEdit; // Удаляем старый текстовый виджет

        ui->pushButton->setText("Check result");
        ui->buttonBox->setEnabled(false);
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
    if (editable) {
        currentWord.setRomanization(pinyinTextEdit->toPlainText());
        currentWord.setTranslation(meaningTextEdit->toPlainText());
        currentWord.setSentence(exampleTextEdit->toPlainText());
        currentWord.setStatus(selectedColor());
    }
    else
    {
        currentWord.setRomanization(pinyinLabel->text());
        currentWord.setTranslation(meaningLabel->text());
        currentWord.setSentence(exampleLabel->text());
        currentWord.setStatus(selectedColor());
    }

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



void EditTermDialog::on_pushButton_clicked()
{
    if (editable) {
        editable = false;
        ui->pushButton->setText("Edit Term");
        //ui->buttonBox->setEnabled(true);

        meaningLabel = new QLabel();
        meaningLabel->setText(meaningTextEdit->toPlainText());
        ui->formLayout_3->replaceWidget(meaningTextEdit, meaningLabel);
        delete meaningTextEdit; // Удаляем старый текстовый виджет

        pinyinLabel = new QLabel(this);
        pinyinLabel->setText(pinyinTextEdit->toPlainText());
        ui->formLayout_3->replaceWidget(pinyinTextEdit, pinyinLabel);
        delete pinyinTextEdit; // Удаляем старый текстовый виджет

        exampleLabel = new QLabel(this);
        exampleLabel->setText(exampleTextEdit->toPlainText());
        ui->formLayout_3->replaceWidget(exampleTextEdit, exampleLabel);
        delete exampleTextEdit; // Удаляем старый текстовый виджет

        meaningLabel->setWordWrap(true);
        pinyinLabel->setWordWrap(true);
        exampleLabel->setWordWrap(true);
    }
    else {
        editable = true;
        ui->pushButton->setText("Check result");
        //ui->buttonBox->setEnabled(false);

        meaningTextEdit = new QPlainTextEdit();
        meaningTextEdit->setPlainText(meaningLabel->text());
        ui->formLayout_3->replaceWidget(meaningLabel, meaningTextEdit);
        delete meaningLabel; // Удаляем старый текстовый виджет

        pinyinTextEdit = new QPlainTextEdit();
        pinyinTextEdit->setPlainText(pinyinLabel->text());
        ui->formLayout_3->replaceWidget(pinyinLabel, pinyinTextEdit);
        delete pinyinLabel; // Удаляем старый текстовый виджет

        exampleTextEdit = new QPlainTextEdit();
        exampleTextEdit->setPlainText(exampleLabel->text());
        ui->formLayout_3->replaceWidget(exampleLabel, exampleTextEdit);
        delete exampleLabel; // Удаляем старый текстовый виджет
    }
}

