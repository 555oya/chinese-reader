#include "customtextedit.h"
#include <qapplication.h>
#include <qstatusbar.h>
#include <QTimer>

void CustomTextEdit::insertSpace()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.insertText(" ");
    setTextCursor(cursor);
    mainWindow->parseText(this->toPlainText());
    mainWindow->getHighlighter()->rehighlight();
}

void CustomTextEdit::joinWords()
{
    QTextCursor cursor = textCursor();
    QString selectedText = cursor.selectedText();
    QRegularExpression regex2("(?<![^一-龥a-zA-Z]) (?![^一-龥a-zA-Z])");
    selectedText.remove(regex2);
    cursor.insertText(selectedText);
    mainWindow->parseText(this->toPlainText());
    mainWindow->getHighlighter()->rehighlight();
}

void CustomTextEdit::useGoogle()
{
    QTextCursor cursor = textCursor();
    QString selectedText = cursor.selectedText();
    selectedText.remove(" ");
    QString urlString = "https://translate.google.com/?ie=UTF-8&sl=zh&tl=ru&text=" + selectedText;
    QDesktopServices::openUrl(QUrl(urlString));
}

void CustomTextEdit::useBaidu()
{
    QTextCursor cursor = textCursor();
    QString selectedText = cursor.selectedText();
    selectedText.remove(" ");
    QString urlString = "https://fanyi.baidu.com/mtpe-individual/multimodal?query=" + selectedText + "&lang=zh2ru";
    QDesktopServices::openUrl(QUrl(urlString));
}

void CustomTextEdit::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QTextCursor cursor = cursorForPosition(event->pos()); // Получаем курсор по позиции клика
        cursor.select(QTextCursor::WordUnderCursor); // Выбираем слово

        QString clickedWord = cursor.selectedText(); // Получаем выделенное слово

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

void CustomTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    rmbEvent = event;
    QMenu *menu = createStandardContextMenu();
    QList<QAction *> actionList = menu->actions();
    qDebug() << "Action list size " << actionList.size();

    menu->addSeparator();
    QAction *insertSpaceAction = new QAction("Insert space", this);
    connect(insertSpaceAction, &QAction::triggered, this, &CustomTextEdit::insertSpace);
    menu->addAction(insertSpaceAction);

    QAction *joinWordsAction = new QAction("Make one word", this);
    connect(joinWordsAction, &QAction::triggered, this, &CustomTextEdit::joinWords);
    menu->addAction(joinWordsAction);

    menu->addSeparator();
    menu->addAction(tr("Translate"))->setEnabled(false);
    menu->addSeparator();

    QAction *useGoogleAction = new QAction("Use google", this);
    connect(useGoogleAction, &QAction::triggered, this, &CustomTextEdit::useGoogle);
    menu->addAction(useGoogleAction);

    QAction *useBaiduAction = new QAction("Use baidu", this);
    connect(useBaiduAction, &QAction::triggered, this, &CustomTextEdit::useBaidu);
    menu->addAction(useBaiduAction);

    QTextCursor cursor = textCursor();
    if(cursor.hasSelection()){
        menu->actions().at(4)->setEnabled(false);
    }
    else {
        menu->actions().at(5)->setEnabled(false);
        menu->actions().at(9)->setEnabled(false);
        menu->actions().at(10)->setEnabled(false);
    }

    menu->exec(event->globalPos());
    delete menu;
}
