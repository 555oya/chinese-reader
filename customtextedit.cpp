#include "customtextedit.h"
#include <qapplication.h>
#include <qstatusbar.h>

void CustomTextEdit::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QTextCursor cursor = cursorForPosition(event->pos()); // Получаем курсор по позиции клика
        cursor.select(QTextCursor::WordUnderCursor); // Выбираем слово

        // Define custom separators (hair space + standard whitespace)
        // QRegularExpression separatorRegex("[\\s\u200B]");

        // // Find word boundaries
        // QString text = toPlainText();
        // int pos = cursor.position();

        // // Find start of word
        // int start = pos;
        // while (start > 0 && !separatorRegex.match(text.mid(start-1, 1)).hasMatch()) {
        //     start--;
        // }

        // // Find end of word
        // int end = pos;
        // while (end < text.length() && !separatorRegex.match(text.mid(end, 1)).hasMatch()) {
        //     end++;
        // }

        // // Select the word
        // cursor.setPosition(start);
        // cursor.setPosition(end, QTextCursor::KeepAnchor);

        QString clickedWord = cursor.selectedText(); // Получаем выделенное слово
        // Remove zero-width spaces and non-Chinese characters
        clickedWord.remove(QChar(0x200B)); // Remove ZWSP

        QRegularExpression removeNonChineseRegex("[^一-龥a-zA-Z]");
        QString cleanedText = clickedWord;
        cleanedText.remove(removeNonChineseRegex);
        if (!cleanedText.isEmpty()) {
            qDebug() << "Clicked word:" << clickedWord; // Выводим в debug

            EditTermDialog dialog(*mainWindow->getWordHashList(), clickedWord, this);
            if (dialog.exec() == QDialog::Accepted) {
                QString chosenColor = dialog.selectedColor();
                WordData currentWord = dialog.getCurrentWord();
                if (!chosenColor.isEmpty()) {
                    emit wordDataHasChanged(clickedWord, chosenColor, currentWord); //сигнал изменения слова
                }
            }
        }
    }

    //QPlainTextEdit::mouseDoubleClickEvent(event); // Передаём событие дальше
}

void CustomTextEdit::mouseMoveEvent(QMouseEvent *event) {
    QTextCursor cursor = cursorForPosition(event->pos()); // Получаем курсор по позиции
    cursor.select(QTextCursor::WordUnderCursor); // Выделяем слово под курсором

    // Define custom separators (hair space + standard whitespace)
    // QRegularExpression separatorRegex("[\\s\u200B]");

    // // Find word boundaries
    // QString text = toPlainText();
    // int pos = cursor.position();

    // // Find start of word
    // int start = pos;
    // while (start > 0 && !separatorRegex.match(text.mid(start-1, 1)).hasMatch()) {
    //     start--;
    // }

    // // Find end of word
    // int end = pos;
    // while (end < text.length() && !separatorRegex.match(text.mid(end, 1)).hasMatch()) {
    //     end++;
    // }

    // // Select the word
    // cursor.setPosition(start);
    // cursor.setPosition(end, QTextCursor::KeepAnchor);

    QString hoveredWord = cursor.selectedText().trimmed(); // Получаем выделенный текст

    QRegularExpression removeNonChineseRegex("[^一-龥a-zA-Z]");
    QString word = hoveredWord;
    word.remove(removeNonChineseRegex);

    if (word != lastHoveredWord && !word.isEmpty()) { // Проверяем, изменилось ли слово
        lastHoveredWord = word; // Обновляем последнее слово
        if (!word.isEmpty()) {
            if (mainWindow) { // Проверяем, что указатель не null
                QString message = word + " — " + mainWindow->getWordHashList()->value(word).getRomanization() + " — " + mainWindow->getWordHashList()->value(word).getTranslation();
                mainWindow->statusBar()->showMessage(message); // Обращаемся к статус-бару
            }
        }
    }
    QPlainTextEdit::mouseMoveEvent(event);
}
