#ifndef EDITTERMDIALOG_H
#define EDITTERMDIALOG_H

#include <QDialog>
#include <QColorDialog>

namespace Ui {
class EditTermDialog;
}

class EditTermDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTermDialog(const QString &word, QWidget *parent = nullptr);
    ~EditTermDialog();

    QString selectedColor(); // Метод для получения выбранного цвета

private slots:
    void on_buttonBox_accepted();
    void on_changeColorBtn_clicked();

private:
    Ui::EditTermDialog *ui;
    QString color; // Хранит выбранный цвет
    //QString color;

    void chooseColor();
};

#endif // EDITTERMDIALOG_H
