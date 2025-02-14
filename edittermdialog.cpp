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
        QColor newColor(170,170,255,125);
        color = newColor;
    }
    if(ui->radioBtnUnknown->isChecked()) {
        QColor newColor(255,83,169,125);
        color = newColor;
    }
    if(ui->radioBtnBetter->isChecked()) {
        QColor newColor(255,85,0,125);
        color = newColor;
    }
    if(ui->radioBtnLearning->isChecked()) {
        QColor newColor(255,255,0,125);
        color = newColor;
    }
    if(ui->radioBtnNearlyKnown->isChecked()) {
        QColor newColor(170,255,0,125);
        color = newColor;
    }
    if(ui->radioBtnKnown->isChecked()) {
        QColor newColor(0,255,0,125);
        color = newColor;
    }
    if(ui->radioBtnIgnore->isChecked()) {
        QColor newColor(140,140,140,125);
        color = newColor;
    }
    if(ui->radioBtnWellKnown->isChecked()) {
        QColor newColor(170,170,255,0);
        color = newColor;
    }

}

QColor EditTermDialog::selectedColor() const {
    return color;
}

void EditTermDialog::on_buttonBox_accepted()
{
    chooseColor();
}


void EditTermDialog::on_changeColorBtn_clicked()
{
    chooseColor();
}

