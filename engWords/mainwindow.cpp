#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "define.hpp"
#include "splitdialog.hpp"
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
{
    ui->setupUi(this);

    setWindowTitle("Eng/Arm wors");
    setWindowIcon(QIcon(":/icons/logo.png"));
    setStyleSheet(MAIN_WINDOW_STYLE);
    setFixedSize(QSize(600, 350));

    ui->sourceBtn->setIcon(QIcon(":/icons/source.png"));
    ui->sourceBtn->setFixedSize(QSize(30, 30));
    ui->sourceBtn->setIconSize(QSize(24, 24));
    ui->sourceBtn->setCursor(Qt::PointingHandCursor);
    ui->sourceBtn->setToolTip("Choose a Source");

    ui->resetBtn->setIcon(QIcon(":/icons/reset_disabled.png"));
    ui->resetBtn->setFixedSize(QSize(30, 30));
    ui->resetBtn->setIconSize(QSize(24, 24));
    ui->resetBtn->setCursor(Qt::PointingHandCursor);
    ui->resetBtn->setToolTip("Reset the Counter");
    ui->resetBtn->setEnabled(false);

    ui->deleteBtn->setIcon(QIcon(":/icons/delete_disabled.png"));
    ui->deleteBtn->setFixedSize(QSize(30, 30));
    ui->deleteBtn->setIconSize(QSize(24, 24));
    ui->deleteBtn->setCursor(Qt::PointingHandCursor);
    ui->deleteBtn->setToolTip("Delete the Word from the Database");
    ui->deleteBtn->setEnabled(false);

    ui->countLabel->setStyleSheet("QLabel { font-size: 20px; color: white; }");
    ui->countLabel->hide();

    ui->newWord->setEnabled(false);
    ui->show->setEnabled(false);

    ui->dltLabel->setStyleSheet("QLabel { font-size: 20px; color: #FF7171; }");
    ui->dltLabel->hide();

    ui->wordCount->setStyleSheet("QLabel { font-size: 14px; color: gray; }");

    _button.push_back(ui->pushButton_0);
    _button.push_back(ui->pushButton_1);
    _button.push_back(ui->pushButton_2);
    _button.push_back(ui->pushButton_3);

    _totalAnswers = 0;
    _rightAnswers = 0;
    _deletedWords = 0;
    _currentWordIndex = -1;

    _dltTimer = new QTimer(this);
    _dltTimer->setSingleShot(true);
    _dltTimer->setInterval(500);

    _nextTimer = new QTimer(this);
    _nextTimer->setSingleShot(true);
    _nextTimer->setInterval(1000);

    connect(ui->sourceBtn, &QPushButton::clicked, this,
            [&](void)
            {
                QString startDir = getExecutableGrandparentDirPath() + "/source";
                // qDebug() << "startDir:" << startDir;
                _sourcePath = QFileDialog::getExistingDirectory(nullptr, "Select Directory", startDir, QFileDialog::ShowDirsOnly);

                if (_sourcePath.isEmpty()) {
                    return;
                }

                auto source = readFromFile(_sourcePath + "/source.txt");
                parse(source);
                writeToFileInAppendMode(_sourcePath + "/words.txt", source);
                _engWords = readFromFile(_sourcePath + "/eng.txt");
                _armWords = readFromFile(_sourcePath + "/arm.txt");

                if (_engWords.size() < 4)
                {
                    ui->wordLabel->setText("Not enough words in eng.txt.\nSelect another source!");
                    return;
                }

                ui->wordCount->setText(QString("Words in base: ") + QString::number(_engWords.size()));

                ui->resetBtn->setIcon(QIcon(":/icons/reset.png"));
                ui->resetBtn->setEnabled(true);
                ui->countLabel->show();

                ui->deleteBtn->setIcon(QIcon(":/icons/delete.png"));
                ui->deleteBtn->setEnabled(true);
                ui->dltLabel->show();

                ui->newWord->setEnabled(true);

                ui->wordLabel->setText("Are you ready?");
                ui->wordLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

                ui->sourceBtn->setIcon(QIcon(":/icons/source_disabled.png"));
                ui->sourceBtn->setEnabled(false);
            });

    connect(ui->newWord, &QPushButton::clicked, this, [this]() { newWordButtonPushAction(); });
    connect(ui->show, &QPushButton::clicked, this, [this]() { showButtonPushAction(); });
    connect(ui->resetBtn, &QPushButton::clicked, this, [this]() { clearButtonPushAction(); });
    connect(ui->deleteBtn, &QPushButton::clicked, this, [this]() { deleteButtonPushAction(); });

    for (int i{}; i < _button.size(); ++i){
        connect(_button[i], &QPushButton::clicked, this, [this, i] { buttonPushAction(i); });
        _button[i]->setEnabled(false);
    }

#ifdef _PLAY_SOUND_

    m_textToSpeech = new TextToSpeech(this);

    ui->soundPlayBtn->setIcon(QIcon(":/icons/soundPlay.png"));
    ui->soundPlayBtn->setFixedSize(QSize(30, 30));
    ui->soundPlayBtn->setIconSize(QSize(24, 24));
    ui->soundPlayBtn->setCursor(Qt::PointingHandCursor);
    ui->soundPlayBtn->setToolTip("Listen");

    connect(ui->soundPlayBtn, &QPushButton::clicked, this,
            [this] { m_textToSpeech->fetchAudio(ui->wordLabel->text()); });

#else
    ui->soundPlayBtn->hide();
#endif

    ui->splitBtn->setIcon(QIcon(":/icons/split.png"));
    ui->splitBtn->setFixedSize(QSize(30, 30));
    ui->splitBtn->setIconSize(QSize(24, 24));
    ui->splitBtn->setCursor(Qt::PointingHandCursor);
    ui->splitBtn->setToolTip("Split a file in 'eng-arm' format into separate files:\n - eng.txt\n - arm.txt");

    connect(ui->splitBtn, &QPushButton::clicked, this,
            [this] {
                SplitDialog dlg(getExecutableGrandparentDirPath() + "/fileToSplit", this);
                dlg.exec();
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
    for (auto& button : _button){
        button->setStyleSheet("QPushButton { background-color: #52606E; }");
        button->setText("");
        button->setEnabled(false);
    }

    _currentWordIndex = getRandomNumber(0, _engWords.size() - 1);
    ui->wordLabel->setText(_engWords[_currentWordIndex]);

    ui->newWord->setEnabled(false);
    ui->show->setEnabled(true);
}

void MainWindow::showButtonPushAction(void)
{
    for (auto& button : _button){
        button->setStyleSheet(MAIN_WINDOW_STYLE);
        button->setEnabled(true);
    }

    _correctIndex = getRandomNumber(0, 3);
    _button[_correctIndex]->setText(_armWords[_currentWordIndex]);

    set<int> buttonIndexes = {0,1,2,3};
    buttonIndexes.erase(_correctIndex);

    set<int> usedIndexes;
    usedIndexes.insert(_currentWordIndex);

    for (int i{}; i < 3; ++i) {
        int index = getRandomNumber(0, _engWords.size() - 1);
        while (usedIndexes.find(index) != usedIndexes.end()){
            index = getRandomNumber(0, _engWords.size() - 1);
        }

        auto it = buttonIndexes.begin();
        _button[*it]->setText(_armWords[index]);
        buttonIndexes.erase(it);
    }

    ui->show->setEnabled(false);
}

void MainWindow::clearButtonPushAction(void)
{
    _totalAnswers = 0;
    _rightAnswers = 0;
    ui->countLabel->setText("0 of 0");
    newWordButtonPushAction();
}

void MainWindow::deleteButtonPushAction(void)
{
    if (_currentWordIndex == -1)
        return;

    this->showButtonPushAction();

    _engWords.removeAt(_currentWordIndex);
    _armWords.removeAt(_currentWordIndex);
    writeToFileInTranscateMode(_sourcePath + "/eng.txt", _engWords);
    writeToFileInTranscateMode(_sourcePath + "/arm.txt", _armWords);
    ui->dltLabel->setText(QString::number(++_deletedWords));

    _button[_correctIndex]->setStyleSheet("QPushButton { background-color: green; color: white; }");
   _dltTimer->start();
    QObject::connect(_dltTimer, &QTimer::timeout, this, [&]() {
        newWordButtonPushAction();
        ui->wordCount->setText(QString("Words in base: ") + QString::number(_engWords.size()));
    });

}

void MainWindow::buttonPushAction(int index)
{
    ++_totalAnswers;
    for (auto& button : _button){
        button->setEnabled(false);
    }

    if (index != _correctIndex) {
        _button[index]->setStyleSheet("QPushButton { background-color: red; color: white; }");
    } else {
        ++_rightAnswers;
    }
    _button[_correctIndex]->setStyleSheet("QPushButton { background-color: green; color: white; }");

    QString text = QString::number(_rightAnswers) + " of " + QString::number(_totalAnswers);
    ui->countLabel->setText(text);

    ui->newWord->setEnabled(true);
    ui->show->setEnabled(true);

    if (index == _correctIndex)
    {
        _nextTimer->start();
        QObject::connect(_nextTimer, &QTimer::timeout, this, [this]() {
            newWordButtonPushAction();
        });
    }
}


