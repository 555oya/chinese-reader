#include "customtextedit.h"

void CustomTextEdit::mousePressEvent(QMouseEvent *event) {
    QTextCursor cursor = cursorForPosition(event->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor); // Выбираем слово

    QString clickedWord = cursor.selectedText(); // Получаем выделенное слово
    QRegularExpression removeNonChineseRegex("[^一-龥a-zA-Z]");
    QString cleanedText = clickedWord;
    cleanedText.remove(removeNonChineseRegex);
    if (!cleanedText.isEmpty()) {
        qDebug() << "Clicked word:" << clickedWord; // Выводим в debug
        // Открываем диалог выбора цвета
        EditTermDialog dialog(clickedWord, this);
        if (dialog.exec() == QDialog::Accepted) {
            QString chosenColor = dialog.selectedColor();
            if (!chosenColor.isEmpty()) {
                emit wordColorSelected(clickedWord, chosenColor); // Сигнал для обновления цвета
            }
        }
    }

    QPlainTextEdit::mousePressEvent(event); // Передаём событие дальше
}
