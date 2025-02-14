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

    QColor selectedColor() const; // Метод для получения выбранного цвета

private slots:
    void on_buttonBox_accepted();
    void on_changeColorBtn_clicked();
    void chooseColor();

private:
    Ui::EditTermDialog *ui;
    QColor color; // Хранит выбранный цвет
};

#endif // EDITTERMDIALOG_H
