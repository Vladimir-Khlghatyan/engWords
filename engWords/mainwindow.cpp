#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "define.hpp"
#include "splitdialog.hpp"
#include "settingsdialog.hpp"
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QToolButton>
#include <QTimer>
#include <QDebug>

#include <random>
#include <set>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_totalAnswers(0)
    , m_rightAnswers(0)
    , m_deletedWords(0)
    , m_currentWordIndex(-1)
{
    ui->setupUi(this);

    setWindowTitle("Eng/Arm words");
    setWindowIcon(QIcon(":/icons/logo.png"));
    setStyleSheet(MAIN_WINDOW_STYLE);
    setFixedSize(QSize(600, 350));

    setUpButtonWithIcon(ui->resetBtn, "reset_disabled", "Reset the Counter", false);
    setUpButtonWithIcon(ui->deleteBtn, "delete_disabled", "Delete the Word from the Database", false);
    setUpButtonWithIcon(ui->sourceBtn, "source", "Choose a Source");

    ui->countLabel->setStyleSheet("QLabel { font-size: 20px; color: white; }");
    ui->countLabel->hide();

    ui->nextBtn->setObjectName("dialog");
    ui->nextBtn->setEnabled(false);

    ui->showBtn->setObjectName("dialog");
    ui->showBtn->setEnabled(false);

    ui->nextBtn->setCursor(Qt::PointingHandCursor);
    ui->showBtn->setCursor(Qt::PointingHandCursor);

    ui->dltLabel->setStyleSheet("QLabel { font-size: 20px; color: #FF8080; }");
    ui->dltLabel->hide();

    ui->wordCount->setStyleSheet("QLabel { font-size: 14px; color: gray; }");
    ui->audioErrorMsg->setStyleSheet("QLabel { font-size: 14px; color: #FF8080; font-weight: normal; font-style: italic; }");

    m_button.push_back(ui->pushButton_0);
    m_button.push_back(ui->pushButton_1);
    m_button.push_back(ui->pushButton_2);
    m_button.push_back(ui->pushButton_3);

    m_dltTimer = new QTimer(this);
    m_dltTimer->setSingleShot(true);
    m_dltTimer->setInterval(500);

    m_nextTimer = new QTimer(this);
    m_nextTimer->setSingleShot(true);
    m_nextTimer->setInterval(1000);

    connect(ui->sourceBtn, &QPushButton::clicked, this, &MainWindow::onSourceButtonClicked);
    connect(ui->nextBtn, &QPushButton::clicked, this, [this]() { newWordButtonPushAction(); });
    connect(ui->showBtn, &QPushButton::clicked, this, [this]() { showButtonPushAction(); });
    connect(ui->resetBtn, &QPushButton::clicked, this, [this]() { clearButtonPushAction(); });
    connect(ui->deleteBtn, &QPushButton::clicked, this, [this]() { deleteButtonPushAction(); });

    for (int i{}; i < m_button.size(); ++i){
        connect(m_button[i], &QPushButton::clicked, this, [this, i] { buttonPushAction(i); });
        m_button[i]->setCursor(Qt::PointingHandCursor);
        m_button[i]->setEnabled(false);
    }

#ifdef _PLAY_SOUND_

    setUpButtonWithIcon(ui->soundPlayBtn, "soundPlay", "Listen");
    connect(ui->soundPlayBtn, &QPushButton::clicked, this, &MainWindow::onSoundPlayButtonClicked);

    m_textToSpeech = new TextToSpeech(this);
    connect(m_textToSpeech, &TextToSpeech::errorOccurred, this, &MainWindow::onTextToSpeechError);

#else
    ui->soundPlayBtn->hide();
#endif

    setUpButtonWithIcon(ui->splitBtn, "split", "Split a file in 'eng-arm' format into separate files:\n - eng.txt\n - arm.txt");
    connect(ui->splitBtn, &QPushButton::clicked, this,
            [this] {
                SplitDialog dlg(getExecutableGrandparentDirPath() + "/fileToSplit", this);
                dlg.exec();
            });

    setUpButtonWithIcon(ui->settingsBtn, "settings", "Settings");
    connect(ui->settingsBtn, &QPushButton::clicked, this,
            [this] {
                SettingsDialog dlg(this);
                if (dlg.exec() == QDialog::Accepted) {
                    qDebug() << "QDialog::Accepted";
                } else {
                    qDebug() << "QDialog::Rejected";
                }
            });

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getExecutableGrandparentDirPath(void)
{
    QString executableDirPath = QCoreApplication::applicationDirPath();
    QDir parentDir(executableDirPath);

    int up{1};
#ifdef Q_OS_MAC
    up += 3;
#endif

    while (up--)
        parentDir.cdUp();
    QString grandparentDirPath = parentDir.absolutePath();
    return grandparentDirPath;
}

QStringList MainWindow::readFromFile(const QString& filePath)
{

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file" << filePath << ":" << file.errorString();
        return {};
    }

    QTextStream in(&file);
    QStringList words;
    while (!in.atEnd()) {
        words.push_back(in.readLine());
    }

    file.close();
    return words;
}

int MainWindow::getRandomNumber(int lower, int upper)
{
    std::random_device rd;
    std::mt19937 eng(rd());

    std::uniform_int_distribution<> distribution(lower, upper);
    int random_number = distribution(eng);

    return random_number;
}

void MainWindow::writeToFileInAppendMode(const QString& filePath, const QStringList& source)
{
    auto currentContent = readFromFile(filePath);
    set<QString> set;
    for (auto& word : currentContent){
        set.insert(word);
    }

    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text))
    {

        QTextStream outFile(&file);
        for (const QString &word : source) {
            if (set.find(word) == set.end()) {
                outFile << word << "\n";
            }
        }
        file.close();
    } else {
        qDebug() << "Failed to open file for writing:" << file.errorString();
    }
}

void MainWindow::writeToFileInTranscateMode(const QString& filePath, const QStringList& source)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {

        QTextStream outFile(&file);
        for (const QString &word : source) {
                outFile << word << "\n";
            }
        file.close();
    } else {
        qDebug() << "Failed to open file for writing:" << file.errorString();
    }
}

void MainWindow::parse(QStringList& source)
{
    set<QString> set;

    for (auto& line : source)
    {
        for (QChar& c : line) {
            c = c.isLetter() ? c.toLower() : ' ';
        }

        auto splitList = line.split(' ');
        for (auto& word : splitList) {
            set.insert(word);
        }
    }

    auto tmpSet = set;

    for (auto& word : tmpSet)
    {
        if (word.size() > 3)
        {
            int suffixSize = word.endsWith("es") ? 2 : (word.endsWith("s") ? 1 : 0);
            if (suffixSize)
            {
                auto substring = word.left(word.size() - suffixSize);
                if (set.find(substring) != set.end()) {
                    set.erase(word);
                }
            }

            // chacking "ed" at the end of the word
            if (word.endsWith("ed"))
            {
                auto substring1 = word.left(word.size() - 1);
                auto substring2 = word.left(word.size() - 2);
                if (set.find(substring1) != set.end() || set.find(substring2) != set.end()) {
                    set.erase(word);
                }
            }

        }
    }

    source.clear();
    for (auto& word : set) {
        if (word.size() > 2) {
            source.push_back(word);
        }
    }
}

void MainWindow::newWordButtonPushAction(void)
{
    for (auto& button : m_button){
        button->setStyleSheet("QPushButton { background-color: #52606E; }");
        button->setText("");
        button->setEnabled(false);
    }

    m_currentWordIndex = getRandomNumber(0, m_engWords.size() - 1);
    ui->wordLabel->setText(m_engWords[m_currentWordIndex]);

    ui->nextBtn->setEnabled(false);
    ui->showBtn->setEnabled(true);
}

void MainWindow::showButtonPushAction(void)
{
    for (auto& button : m_button){
        button->setStyleSheet(MAIN_WINDOW_STYLE);
        button->setEnabled(true);
    }

    m_correctIndex = getRandomNumber(0, 3);
    m_button[m_correctIndex]->setText(m_armWords[m_currentWordIndex]);

    set<int> buttonIndexes = {0,1,2,3};
    buttonIndexes.erase(m_correctIndex);

    set<int> usedIndexes;
    usedIndexes.insert(m_currentWordIndex);

    for (int i{}; i < 3; ++i) {
        int index = getRandomNumber(0, m_engWords.size() - 1);
        while (usedIndexes.find(index) != usedIndexes.end()){
            index = getRandomNumber(0, m_engWords.size() - 1);
        }

        auto it = buttonIndexes.begin();
        m_button[*it]->setText(m_armWords[index]);
        buttonIndexes.erase(it);
    }

    ui->showBtn->setEnabled(false);
}

void MainWindow::clearButtonPushAction(void)
{
    m_totalAnswers = 0;
    m_rightAnswers = 0;
    ui->countLabel->setText("0 of 0");
    newWordButtonPushAction();
}

void MainWindow::deleteButtonPushAction(void)
{
    if (m_currentWordIndex == -1)
        return;

    this->showButtonPushAction();

    m_engWords.removeAt(m_currentWordIndex);
    m_armWords.removeAt(m_currentWordIndex);
    writeToFileInTranscateMode(m_sourcePath + "/eng.txt", m_engWords);
    writeToFileInTranscateMode(m_sourcePath + "/arm.txt", m_armWords);
    ui->dltLabel->setText(QString::number(++m_deletedWords));

    m_button[m_correctIndex]->setStyleSheet("QPushButton { background-color: green; color: white; }");
   m_dltTimer->start();
    QObject::connect(m_dltTimer, &QTimer::timeout, this, [&]() {
        newWordButtonPushAction();
        ui->wordCount->setText(QString("Words in base: ") + QString::number(m_engWords.size()));
    });

}

void MainWindow::buttonPushAction(int index)
{
    ++m_totalAnswers;
    for (auto& button : m_button){
        button->setEnabled(false);
    }

    if (index != m_correctIndex) {
        m_button[index]->setStyleSheet("QPushButton { background-color: red; color: white; }");
    } else {
        ++m_rightAnswers;
    }
    m_button[m_correctIndex]->setStyleSheet("QPushButton { background-color: green; color: white; }");

    QString text = QString::number(m_rightAnswers) + " of " + QString::number(m_totalAnswers);
    ui->countLabel->setText(text);

    ui->nextBtn->setEnabled(true);
    ui->showBtn->setEnabled(true);

    if (index == m_correctIndex)
    {
        m_nextTimer->start();
        QObject::connect(m_nextTimer, &QTimer::timeout, this, [this]() {
            newWordButtonPushAction();
        });
    }
}

#ifdef _PLAY_SOUND_
void MainWindow::onTextToSpeechError(const QString& msg)
{
    ui->audioErrorMsg->setText(msg);
    QTimer::singleShot(3000, this, [this]() {
        ui->audioErrorMsg->setText("");
    });
}
#endif

void MainWindow::onSourceButtonClicked()
{
    QString startDir = getExecutableGrandparentDirPath() + "/source";
    m_sourcePath = QFileDialog::getExistingDirectory(nullptr, "Select Directory", startDir, QFileDialog::ShowDirsOnly);

    if (m_sourcePath.isEmpty()) {
        return;
    }

    auto source = readFromFile(m_sourcePath + "/source.txt");
    parse(source);
    writeToFileInAppendMode(m_sourcePath + "/words.txt", source);
    m_engWords = readFromFile(m_sourcePath + "/eng.txt");
    m_armWords = readFromFile(m_sourcePath + "/arm.txt");

    if (m_engWords.size() < 4)
    {
        ui->wordLabel->setText("Not enough words in eng.txt.\nSelect another source!");
        return;
    }

    ui->wordCount->setText(QString("Words in base: ") + QString::number(m_engWords.size()));

    ui->resetBtn->setIcon(QIcon(":/icons/reset.png"));
    ui->resetBtn->setEnabled(true);
    ui->countLabel->show();

    ui->deleteBtn->setIcon(QIcon(":/icons/delete.png"));
    ui->deleteBtn->setEnabled(true);
    ui->dltLabel->show();

    ui->nextBtn->setEnabled(true);

    ui->wordLabel->setText("Are you ready?");
    ui->wordLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    ui->sourceBtn->setIcon(QIcon(":/icons/source_disabled.png"));
    ui->sourceBtn->setEnabled(false);
}

void MainWindow::onSoundPlayButtonClicked()
{
    ui->audioErrorMsg->setText("");
    m_textToSpeech->fetchAudio(ui->wordLabel->text());
}

void MainWindow::setUpButtonWithIcon(QPushButton* btn, const QString& iconName, const QString& tooltip,
                                        bool isEnabled, const QSize& buttonSize, const QSize& iconSize)
{
    btn->setIcon(QIcon(":/icons/" + iconName + ".png"));
    btn->setFixedSize(buttonSize);
    btn->setIconSize(iconSize);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setToolTip(tooltip);
    btn->setEnabled(isEnabled);
}

