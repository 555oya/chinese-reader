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

EditTermDialog::~EditTermDialog()
{
    delete ui;
}

void EditTermDialog::chooseColor() {
    // QColor newColor = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    // newColor.setAlpha(125);
    // if (newColor.isValid()) {
    //     color = newColor;
    //     ui->wordLabel->setStyleSheet("background-color: " + color.name() + ";"); // Меняем фон слова
    // }

    if(ui->radioBtnNew->isChecked()) {
        color = "new";
    }
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

void EditTermDialog::on_buttonBox_accepted()
{
    //chooseColor();
}


void EditTermDialog::on_changeColorBtn_clicked()
{
    //chooseColor();
}

