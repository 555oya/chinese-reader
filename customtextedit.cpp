#include "customtextedit.h"
#include <qapplication.h>
#include <qstatusbar.h>
#include <QTimer>

void CustomTextEdit::insertSpace()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.insertText(" ");
    setTextCursor(cursor);
    emit(textHasChanged(this->toPlainText()));
}

void CustomTextEdit::joinWords()
{
    QTextCursor cursor = textCursor();
    QString selectedText = cursor.selectedText();
    QRegularExpression regex2("(?<![^一-龥a-zA-Z]) (?![^一-龥a-zA-Z])");
    selectedText.remove(regex2);
    cursor.insertText(selectedText);
    emit(textHasChanged(this->toPlainText()));
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

void CustomTextEdit::addNRDict()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor);
    QString clickedWord = cursor.selectedText();
    QFile file("dict/user.dict.utf8");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << clickedWord << " 27 nr\n";
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл user.dict.utf8 для записи!";
    }
}

void CustomTextEdit::addNSDict()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor);
    QString clickedWord = cursor.selectedText();
    QFile file("dict/user.dict.utf8");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << clickedWord << " 116 ns";
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл user.dict.utf8 для записи!";
    }
}

void CustomTextEdit::addNTDict()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor);
    QString clickedWord = cursor.selectedText();
    QFile file("dict/user.dict.utf8");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << clickedWord << " 70 nt";
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл user.dict.utf8 для записи!";
    }
}

void CustomTextEdit::addNDict()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor);
    QString clickedWord = cursor.selectedText();
    QFile file("dict/user.dict.utf8");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << clickedWord << " 125 n";
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл user.dict.utf8 для записи!";
    }
}

void CustomTextEdit::addNZDict()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor);
    QString clickedWord = cursor.selectedText();
    QFile file("dict/user.dict.utf8");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << clickedWord << " 41 nz";
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл user.dict.utf8 для записи!";
    }
}

void CustomTextEdit::addVDict()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor);
    QString clickedWord = cursor.selectedText();
    QFile file("dict/user.dict.utf8");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << clickedWord << " 368 v";
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл user.dict.utf8 для записи!";
    }
}

void CustomTextEdit::addVNDict()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor);
    QString clickedWord = cursor.selectedText();
    QFile file("dict/user.dict.utf8");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << clickedWord << " 390 vn";
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл user.dict.utf8 для записи!";
    }
}

void CustomTextEdit::addVDDict()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor);
    QString clickedWord = cursor.selectedText();
    QFile file("dict/user.dict.utf8");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << clickedWord << " 1581 vd";
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл user.dict.utf8 для записи!";
    }
}

void CustomTextEdit::addADict()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor);
    QString clickedWord = cursor.selectedText();
    QFile file("dict/user.dict.utf8");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << clickedWord << " 481 a";
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл user.dict.utf8 для записи!";
    }
}

void CustomTextEdit::addDDict()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor);
    QString clickedWord = cursor.selectedText();
    QFile file("dict/user.dict.utf8");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << clickedWord << " 1633 d";
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл user.dict.utf8 для записи!";
    }
}

void CustomTextEdit::addIDict()
{
    QTextCursor cursor = cursorForPosition(rmbEvent->pos()); // Получаем курсор по позиции клика
    cursor.select(QTextCursor::WordUnderCursor);
    QString clickedWord = cursor.selectedText();
    QFile file("dict/user.dict.utf8");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << clickedWord << " 12 i";
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл user.dict.utf8 для записи!";
    }
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
            EditTermDialog dialog(dbManager, clickedWord, this);
            if (dialog.exec() == QDialog::Accepted) {
                qDebug() << "Accepted";
                QString chosenColor = dialog.selectedColor();
                WordData currentWord = dialog.getCurrentWord();
                if (!chosenColor.isEmpty()) {
                    emit wordDataHasChanged(clickedWord, chosenColor, currentWord); //сигнал изменения слова
                }
            }
        }
    }
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
            if (dbManager->wordExists(word)) { // Проверяем, что указатель не null
                QString message = word + " — " + dbManager->getWord(word).getRomanization() + " — " + dbManager->getWord(word).getTranslation();
                statusBar->showMessage(message); // Обращаемся к статус-бару
            }
            else {
                QString message = word + " — " + " — ";
                statusBar->showMessage(message); // Обращаемся к статус-бару
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

    QMenu* submenu = menu->addMenu( "Add to dict as..." );

    QAction *addNR = new QAction("Person name", this);
    connect(addNR, &QAction::triggered, this, &CustomTextEdit::addNRDict);
    submenu->addAction(addNR);

    QAction *addNS = new QAction("Place name", this);
    connect(addNS, &QAction::triggered, this, &CustomTextEdit::addNSDict);
    submenu->addAction(addNS);

    QAction *addNT = new QAction("Organization Name", this);
    connect(addNT, &QAction::triggered, this, &CustomTextEdit::addNTDict);
    submenu->addAction(addNT);

    // QAction *addNW = new QAction("作品名", this);
    // connect(addNW, &QAction::triggered, this, &CustomTextEdit::addToDictionary);
    // submenu->addAction(addNW);

    QAction *addN = new QAction("Common Noun", this);
    connect(addN, &QAction::triggered, this, &CustomTextEdit::addNDict);
    submenu->addAction(addN);

    QAction *addNZ = new QAction("Other proper names", this);
    connect(addNZ, &QAction::triggered, this, &CustomTextEdit::addNZDict);
    submenu->addAction(addNZ);

    submenu->addSeparator();

    QAction *addV = new QAction("Common Verbs", this);
    connect(addV, &QAction::triggered, this, &CustomTextEdit::addVDict);
    submenu->addAction(addV);

    QAction *addVD = new QAction("Converb", this);
    connect(addVD, &QAction::triggered, this, &CustomTextEdit::addVDDict);
    submenu->addAction(addVD);

    QAction *addVN = new QAction("Noun Verb", this);
    connect(addVN, &QAction::triggered, this, &CustomTextEdit::addVNDict);
    submenu->addAction(addVN);

    submenu->addSeparator();

    QAction *addA = new QAction("Аdjective", this);
    connect(addA, &QAction::triggered, this, &CustomTextEdit::addADict);
    submenu->addAction(addA);

    QAction *addD = new QAction("Adverb", this);
    connect(addD, &QAction::triggered, this, &CustomTextEdit::addDDict);
    submenu->addAction(addD);

    QAction *addI = new QAction("Chengyu", this);
    connect(addI, &QAction::triggered, this, &CustomTextEdit::addIDict);
    submenu->addAction(addI);

    QTextCursor cursor = textCursor();
    if(cursor.hasSelection()){
        menu->actions().at(4)->setEnabled(false);
        menu->actions().at(11)->setEnabled(false);
    }
    else {
        menu->actions().at(5)->setEnabled(false);
        menu->actions().at(9)->setEnabled(false);
        menu->actions().at(10)->setEnabled(false);
    }

    if(document()->isEmpty()){
        for (auto &action : menu->actions()) {
            action->setEnabled(false);
        }
    }

    menu->exec(event->globalPos());
    delete menu;
}
