#include "mainwindow.h"
#include "customtextedit.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "settingsdialog.h"
#include "wordhighlighter.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showNormal();  // Восстанавливает нормальное состояние
    settings = new QSettings(this);
    loadSettings();

    applyTheme(settings->value("app/theme", "dark").toString());
    if (!defaultFolderSet) {
        QMessageBox::information(this, "Choose folder", "Select a directory for CharacTree", QMessageBox::Ok);
        folderPath = QFileDialog::getExistingDirectory(this, tr("Choose directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        //проверка
        folderPath = folderPath + "/charactree";
        defaultOpenFileFolderPath = folderPath + "/texts";
        QDir dirText(defaultOpenFileFolderPath);
        if (!dirText.exists())
            dirText.mkpath(defaultOpenFileFolderPath);
        QDir dirTerm(termDictFolderPath);
        if (!dirTerm.exists())
            dirTerm.mkpath(termDictFolderPath);
    }

    defaultOpenFileFolderPath = folderPath + "/texts";
    QString exeDir = QCoreApplication::applicationDirPath();
    databasePath = exeDir + "/database.db";

    dbManager = new DbManager(databasePath, this);

    if (!dbManager->openDatabase()) {
        QMessageBox::warning(this, "Database Error",
                             QString("Cannot open database: %1").arg(dbManager->getLastError()));
    }

    if (!dbManager->createTables()) {
        QMessageBox::warning(this, "Database Error",
                             QString("Cannot create tables: %1").arg(dbManager->getLastError()));
    }

    CustomTextEdit *customTextEdit = new CustomTextEdit(dbManager,ui->statusbar);
    ui->gridLayout->replaceWidget(ui->textEdit, customTextEdit);
    delete ui->textEdit; // Удаляем старый текстовый виджет
    ui->textEdit = customTextEdit;
    QFont font("SimSun", 20);
    ui->textEdit->setFont(font);

    currentText = Text();

    connect(customTextEdit, &CustomTextEdit::wordDataHasChanged, this, &MainWindow::updateWordData);
    connect(customTextEdit, &CustomTextEdit::textHasChanged, this, &MainWindow::updateText);

    changesInWord = false;

    qDebug() << "defaultFolderSet " << defaultOpenFileFolderPath;

    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(true);
    ui->dictionaryButton->show();

    int max_width = ui->nearlyUnknownWordsPercent->width();
    ui->label->setFixedWidth(max_width);
    ui->label_8->setFixedWidth(max_width);
    ui->label_2->setFixedWidth(max_width);
    ui->label_3->setFixedWidth(max_width);
    ui->label_4->setFixedWidth(max_width);
    ui->label_5->setFixedWidth(max_width);
    ui->label_6->setFixedWidth(max_width);
    ui->label_7->setFixedWidth(max_width);

    highlighter = new WordHighlighter(dbManager, customTextEdit->document());
    highlighter->setSpaceSize(1);
}

MainWindow::~MainWindow()
{
    saveSettings();
    qDebug() << (settings->value("folderPath", "")).toString();
    delete ui;
    delete settings;
    delete highlighter;
    delete dbManager;  // Удаляем менеджер базы данных
}

void MainWindow::getStatistics()
{
    ui->newWordsPercent->setText(QString::number(currentText.getWordsPercent("new"), 'f', 2) + "%");
    ui->unknownWordsPercent->setText(QString::number(currentText.getWordsPercent("unknown"), 'f', 2) + "%");
    ui->nearlyUnknownWordsPercent->setText(QString::number(currentText.getWordsPercent("nearlyUnknown"), 'f', 2) + "%");
    ui->learningWordsPercent->setText(QString::number(currentText.getWordsPercent("learning"), 'f', 2) + "%");
    ui->nearlyKnownWordsPercent->setText(QString::number(currentText.getWordsPercent("nearlyKnown"), 'f', 2) + "%");
    ui->knownWordsPercent->setText(QString::number(currentText.getWordsPercent("known"), 'f', 2) + "%");
    ui->ignoredWordsPercent->setText(QString::number(currentText.getWordsPercent("ignored"), 'f', 2) + "%");
    ui->wellKnownWordsPercent->setText(QString::number(currentText.getWordsPercent("wellKnown"), 'f', 2) + "%");
    ui->textReadability->setText(QString::number(currentText.getTextReadability(), 'f', 2) + "%");
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), defaultOpenFileFolderPath, tr("Text files (*.txt)"));
    qDebug() << "Text folder " << defaultOpenFileFolderPath;
    currentText = Text(true, fileName, dbManager);

    currentText.cutToWords();

    this->ui->textEdit->setPlainText(currentText.getTextStr());
    highlighter->rehighlight();

    getStatistics();
    //ui->pushButton_2->setEnabled(true);
    getStatistics();
    ui->groupBox->setHidden(false);
    ui->groupBox_2->setHidden(false);
}

void MainWindow::updateWordData(const QString &word, const QString &color, WordData &currentWord) {
    qDebug() << "Changing color for word:" << word << " to " << color;

    highlighter->rehighlight();
    getStatistics();
}

void MainWindow::updateText(const QString &newText)
{
    currentText = Text(false, newText, dbManager);
    highlighter->rehighlight();
    getStatistics();
}

void MainWindow::saveSettings()
{
    settings->setValue("folderPath", folderPath);
    settings->setValue("defaultFolderSet", true);
}

void MainWindow::loadSettings()
{
    folderPath = settings->value("folderPath", "").toString();
    defaultFolderSet = settings->value("defaultFolderSet").toBool();

    qDebug() << "folder path: " << folderPath;
}

void MainWindow::on_checkBoxHideSpaces_checkStateChanged(const Qt::CheckState &arg1)
{
    if (arg1 == Qt::Checked)
        highlighter->setSpaceSize(1);
    else
        highlighter->setSpaceSize(20);
    highlighter->rehighlight();
}

void MainWindow::on_dictionaryButton_clicked()
{
    if (!dbManager) {
        QMessageBox::warning(this, "Error", "Database not initialized");
        return;
    }

    DictionaryDialog *dialog = new DictionaryDialog(dbManager, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}


void MainWindow::on_settingsButton_clicked()
{
    SettingsDialog dialog(settings, this);
    connect(&dialog, &SettingsDialog::settingsChanged, this, &MainWindow::applySettings);

    if (dialog.exec() == QDialog::Accepted) {
        applySettings();
    }
}

void MainWindow::applySettings()
{
    // Применяем шрифт
    QString fontFamily = settings->value("font/family", "SimSun").toString();
    int fontSize = settings->value("font/size", 20).toInt();
    QFont font(fontFamily, fontSize);
    ui->textEdit->setFont(font);

    // Применяем тему
    QString theme = settings->value("app/theme", "light").toString();
    applyTheme(theme);

    // Обновляем пути если изменилась папка
    QString newFolderPath = settings->value("folderPath", "").toString();
    if (!newFolderPath.isEmpty() && newFolderPath != folderPath) {
        folderPath = newFolderPath;
        defaultOpenFileFolderPath = folderPath + "/texts";
        databasePath = folderPath + "/database.db";

        // Обновляем пути в базе данных
        if (dbManager) {
            delete dbManager;
            dbManager = new DbManager(databasePath, this);

            if (!dbManager->openDatabase()) {
                QMessageBox::warning(this, "Database Error",
                                     QString("Cannot open database: %1").arg(dbManager->getLastError()));
            }

            if (!dbManager->createTables()) {
                QMessageBox::warning(this, "Database Error",
                                     QString("Cannot create tables: %1").arg(dbManager->getLastError()));
            }
        }
    }
}

void MainWindow::applyTheme(const QString &theme)
{
    // Полностью убираем кастомные стили
    qApp->setStyleSheet("");

    if (theme == "dark") {
        // Используем системную темную тему
        qApp->setStyle("Fusion");

        QPalette darkPalette;

        // Базовые цвета для темной темы (аналоги системным)
        darkPalette.setColor(QPalette::Window, QColor(32, 32, 32));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(35, 35, 35));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        darkPalette.setColor(QPalette::Active, QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
        darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, Qt::darkGray);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
        darkPalette.setColor(QPalette::Disabled, QPalette::Light, QColor(53, 53, 53));

        qApp->setPalette(darkPalette);

    } else if (theme == "light") {
        // Используем системную светлую тему
        qApp->setStyle("Fusion");

        QPalette LightPalette;

        // Базовые цвета для светлой темы
        LightPalette.setColor(QPalette::Window, QColor(240, 243, 246)); // Мягкий голубовато-серый фон
        LightPalette.setColor(QPalette::WindowText, QColor(33, 33, 33)); // Темно-серый текст
        LightPalette.setColor(QPalette::Base, QColor(255, 255, 255)); // Белый фон для текстовых полей
        LightPalette.setColor(QPalette::AlternateBase, QColor(248, 249, 250)); // Альтернативный фон (для таблиц)
        LightPalette.setColor(QPalette::ToolTipBase, QColor(33, 33, 33)); // Белый фон подсказок
        LightPalette.setColor(QPalette::ToolTipText, QColor(33, 33, 33)); // Темный текст подсказок
        LightPalette.setColor(QPalette::Text, QColor(33, 33, 33)); // Основной текст
        LightPalette.setColor(QPalette::Button, QColor(245, 247, 249)); // Светло-серый фон кнопок
        LightPalette.setColor(QPalette::ButtonText, QColor(41, 41, 41)); // Темно-серый текст кнопок
        LightPalette.setColor(QPalette::BrightText, QColor(220, 53, 69)); // Яркий текст (красный для ошибок)
        LightPalette.setColor(QPalette::Link, QColor(66, 133, 244)); // Приятный синий для ссылок (Google синий)
        LightPalette.setColor(QPalette::Highlight, QColor(66, 133, 244)); // Синий цвет выделения
        LightPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255)); // Белый текст на выделении

        // Активные/неактивные состояния
        LightPalette.setColor(QPalette::Active, QPalette::Button, QColor(235, 238, 241)); // Активная кнопка чуть темнее
        LightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(158, 158, 158)); // Серый для неактивного текста кнопок
        LightPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(158, 158, 158)); // Серый для неактивного текста окон
        LightPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(158, 158, 158)); // Серый для неактивного текста
        LightPalette.setColor(QPalette::Disabled, QPalette::Light, QColor(245, 247, 249)); // Свет для неактивных элементов

        qApp->setPalette(LightPalette);
    } else if (theme == "pink") {
        // Используем системную светлую тему
        qApp->setStyle("Fusion");

        QPalette LightPalette;

        LightPalette.setColor(QPalette::Window, QColor(250, 240, 250)); // Светло-лиловый
        LightPalette.setColor(QPalette::WindowText, QColor(90, 65, 95)); // Темно-лиловый
        LightPalette.setColor(QPalette::Base, QColor(255, 248, 255));
        LightPalette.setColor(QPalette::AlternateBase, QColor(248, 235, 248));
        LightPalette.setColor(QPalette::ToolTipBase, QColor(245, 230, 245));
        LightPalette.setColor(QPalette::ToolTipText, QColor(110, 80, 115));
        LightPalette.setColor(QPalette::Text, QColor(95, 70, 100));
        LightPalette.setColor(QPalette::Button, QColor(245, 230, 240)); // Розово-лиловый
        LightPalette.setColor(QPalette::ButtonText, QColor(105, 75, 110));
        LightPalette.setColor(QPalette::BrightText, QColor(220, 110, 180)); // Яркий пурпурный
        LightPalette.setColor(QPalette::Link, QColor(210, 100, 170));
        LightPalette.setColor(QPalette::Highlight, QColor(210, 100, 170));
        LightPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));

        // Активные/неактивные состояния
        LightPalette.setColor(QPalette::Active, QPalette::Button, QColor(245, 220, 230)); // Активная кнопка - чуть темнее
        LightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(180, 160, 170)); // Серо-розовый для неактивного
        LightPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(180, 160, 170));
        LightPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(180, 160, 170));
        LightPalette.setColor(QPalette::Disabled, QPalette::Light, QColor(250, 235, 242));

        qApp->setPalette(LightPalette);
    }
}
