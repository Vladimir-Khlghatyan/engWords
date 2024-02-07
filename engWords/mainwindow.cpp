#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    this->setStyleSheet(R"(QMainWindow {
                            background: solid black;
                        }
                        QLabel {
                            font-size:   32px;
                            font-weight: bold;
                            color:       white;
                        }
                        QPushButton {
                            background-color: #52606E;
                            color:            white;
                            font-size:        20px;
                            font-weight:      bold;
                            border-radius:    10px;
                        }
                        QPushButton:hover {
                            background-color: #708294;
                            color:            white;
                            font-size:        20px;
                            font-weight:      bold;
                        })");

    ui->source->setStyleSheet(R"(QPushButton {
                                    font-size:  12px;
                                })");

    ui->clear->setEnabled(false);
    ui->clear->setStyleSheet(R"(QPushButton {
                                    font-size:  14px;
                                    color:      gray;
                                })");

    ui->dlt->setEnabled(false);
    ui->dlt->setStyleSheet(R"(QPushButton {
                                    font-size:  14px;
                                    color:      gray;
                                })");

    ui->countLabel->setStyleSheet(R"(QLabel {
                                        font-size:  20px;
                                        color:      gray;
                                    })");

    ui->newWord->setEnabled(false);
    ui->newWord->setStyleSheet(R"(QPushButton {
                                    color: gray;
                                })");

    ui->dltLabel->hide();
    ui->dltLabel->setStyleSheet(R"(QLabel {
                                        font-size:  20px;
                                        color:      #FF3333;
                                    })");

    ui->wordCount->setStyleSheet(R"(QLabel {
                                        font-size:  14px;
                                        color:      gray;
                                    })");


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

    ui->checkBox->hide();
    connect(ui->source, &QToolButton::clicked, this,
            [&](void)
            {
                QString startDir = getExecutableGrandparentDirPath() + "/source";
                qDebug() << "startDir:" << startDir;
                _sourcePath = QFileDialog::getExistingDirectory(nullptr, "Select Directory", startDir, QFileDialog::ShowDirsOnly);

                if (_sourcePath.isEmpty())
                    return;

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

                ui->clear->setEnabled(true);
                ui->clear->setStyleSheet(R"(QPushButton {
                                            font-size:  14px;
                                            color:      white;
                                        })");

                ui->countLabel->setStyleSheet(R"(QLabel {
                                                    font-size:  20px;
                                                    color:      white;
                                                })");

                ui->checkBox->show();

                ui->newWord->setEnabled(true);
                ui->newWord->setStyleSheet(R"(QPushButton {
                                                color: white;
                                            })");

                ui->wordLabel->setText("Are you ready?");

                ui->source->setEnabled(false);
                ui->source->setStyleSheet(R"(QPushButton {
                                                font-size:  14px;
                                                color:      gray;
                                            })");
            });

    connect(ui->newWord, &QToolButton::clicked, this, [&](void) { this->newWordButtonPushAction(); });
    connect(ui->clear, &QToolButton::clicked, this, [&](void) { this->clearButtonPushAction(); });
    connect(ui->dlt, &QToolButton::clicked, this, [&](void) { this->deleteButtonPushAction(); });

    for (int i{}; i < _button.size(); ++i){
        connect(_button[i], &QToolButton::clicked, this, [this, i] { this->buttonPushAction(i); });
        _button[i]->setEnabled(false);
    }

    connect(ui->checkBox, &QCheckBox::stateChanged, this,
            [&](void)
            {
                if (ui->checkBox->isChecked())
                {
                    ui->dlt->setEnabled(true);
                    ui->dlt->setStyleSheet(R"(QPushButton {
                                                font-size:  14px;
                                                color:      white;
                                            })");
                    ui->dltLabel->show();
                }
                else
                {
                    ui->dlt->setEnabled(false);
                    ui->dlt->setStyleSheet(R"(QPushButton {
                                                font-size:  14px;
                                                color:      gray;
                                            })");
                    ui->dltLabel->hide();
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
    for (auto& button : _button){
        button->setStyleSheet(R"(QPushButton {
                                    background-color: #52606E;
                                    color:            white;
                                    font-size:        20px;
                                    font-weight:      bold;
                                }
                                QPushButton:hover {
                                    background-color: #708294;
                                    color:            white;
                                    font-size:        20px;
                                    font-weight:      bold;
                                })");
        button->setEnabled(true);
    }

    _currentWordIndex = getRandomNumber(0, _engWords.size() - 1);
    ui->wordLabel->setText(_engWords[_currentWordIndex]);

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
    ui->newWord->setStyleSheet(R"(QPushButton {
                                    color: gray;
                                })");
    ui->newWord->setEnabled(false);
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

    _engWords.removeAt(_currentWordIndex);
    _armWords.removeAt(_currentWordIndex);
    writeToFileInTranscateMode(_sourcePath + "/eng.txt", _engWords);
    writeToFileInTranscateMode(_sourcePath + "/arm.txt", _armWords);
    ui->dltLabel->setText(QString::number(++_deletedWords));

    _button[_correctIndex]->setStyleSheet("QPushButton { background-color: green;}");
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
        _button[index]->setStyleSheet("QPushButton { background-color: red;}");
    } else {
        ++_rightAnswers;
    }
    _button[_correctIndex]->setStyleSheet("QPushButton { background-color: green;}");

    QString text = QString::number(_rightAnswers) + " of " + QString::number(_totalAnswers);
    ui->countLabel->setText(text);

    ui->newWord->setStyleSheet(R"(QPushButton {
                                    color: white;
                                })");
    ui->newWord->setEnabled(true);

    _nextTimer->start();
    QObject::connect(_nextTimer, &QTimer::timeout, this, [&]() {
        newWordButtonPushAction();
    });
}


