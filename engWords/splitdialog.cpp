#include "splitdialog.hpp"
#include "mainwindow.h"

#include <fstream>
#include <vector>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>

SplitDialog::SplitDialog(const QString& path, QWidget* parent)
    : baseClass(parent)
    , m_path(path)
{
    setWindowIcon(QIcon(":/icons/split.png"));
    setWindowTitle("Split file");
    setMinimumSize(400,100);

    m_text = new QLabel("Choose a file in 'eng-arm' format for splitting.", this);
    m_text->setObjectName("dialog");    

    m_browsBtn = new QPushButton("", this);
    qobject_cast<MainWindow*>(parent)->setUpButtonWithIcon(m_browsBtn, "source", "Choose a file.");
    connect(m_browsBtn, &QPushButton::clicked, this, &SplitDialog::onBrows);

    m_cancelBtn = new QPushButton("Cancel", this);
    m_cancelBtn->setCursor(Qt::PointingHandCursor);
    m_cancelBtn->setObjectName("dialog");
    connect(m_cancelBtn, &QPushButton::clicked, this, &SplitDialog::onCencel);

    m_splitBtn = new QPushButton("Split", this);
    m_splitBtn->setCursor(Qt::PointingHandCursor);
    m_splitBtn->setObjectName("dialog");
    m_splitBtn->setEnabled(false);
    connect(m_splitBtn, &QPushButton::clicked, this, &SplitDialog::onSplit);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(m_browsBtn, 0, Qt::AlignLeft);
    hLayout->addSpacerItem(new QSpacerItem(40,20));
    hLayout->addWidget(m_cancelBtn);
    hLayout->addWidget(m_splitBtn);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(m_text, 0, Qt::AlignCenter);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
}

void SplitDialog::onBrows()
{
    m_filePath = QFileDialog::getOpenFileName(nullptr, "Select File", m_path, "*.txt");

    if (m_filePath.isEmpty()) {
        m_text->setText("Choose a file in 'eng-arm' format for splitting.");
    } else {
        m_text->setText("The file is chosen for splitting.");
    }

    m_text->setStyleSheet("color: white;");
    m_splitBtn->setEnabled(!m_filePath.isEmpty());
}

void SplitDialog::onCencel()
{
    reject();
}

void SplitDialog::onSplit()
{
    if (m_filePath.isEmpty()) {
        return;
    }

    parseTheFile();

    if (m_errorText.empty())
    {
        m_text->setText("Succeeded.");
        m_text->setStyleSheet("color: green;");
    }
    else
    {
        m_text->setText(m_errorText.c_str());
        m_text->setStyleSheet("color: red;");
    }
}

void SplitDialog::parseTheFile()
{
    std::ifstream sourceFile(m_filePath.toStdString());

    m_errorText.clear();
    if (!sourceFile.is_open())
    {
        m_errorText = "Failed to open the file.";
        return;
    }

    int line{};
    std::vector<std::string> engWords, armWords;
    std::string lineStr;
    while (std::getline(sourceFile, lineStr))
    {
        ++line;
        if (lineStr.empty()) {
            continue;
        }

        auto [engWord, armWord] = getWordsPair(lineStr);
        if (engWord.empty())
        {
            m_errorText = "Error on line " + std::to_string(line) + " >>> [ ";
            m_errorText += (lineStr.size() < 28) ? lineStr + " ]" : lineStr.substr(0, 24) + "... ]";
            sourceFile.close();
            return;
        }
        engWords.push_back(engWord);
        armWords.push_back(armWord);
    }

    sourceFile.close();

    if (engWords.empty())
    {
        m_errorText = "The file is empty.";
        return;
    }

    assert(engWords.size() == armWords.size());

    std::string savePath(m_filePath.toStdString());
    for (int i = savePath.size() - 1; i >= 0; --i)
    {
        if (savePath[i] == '/' || savePath[i] == '\\') {
            break;
        }
        savePath.pop_back();
    }

    std::ofstream engFile(savePath + "eng.txt", std::ios::app); // std::ios::app/end/
    std::ofstream armFile(savePath + "arm.txt", std::ios::app); // std::ios::app/end/
    if (!engFile.is_open() || !armFile.is_open())
    {
        m_errorText = "Failed to open or create new file.";
        return;
    }

    for (int i{}; i < engWords.size(); ++i)
    {
        engFile << engWords[i] << std::endl;
        armFile << armWords[i] << std::endl;
    }

    engFile.close();
    armFile.close();
}

std::pair<std::string, std::string> SplitDialog::getWordsPair(const std::string& str)
{
    std::vector<std::string> words;

    int start = str.find_first_not_of(" ");
    while (start != std::string::npos)
    {
        int end = str.find_first_of(" ", start);
        if (end == std::string::npos) {
            end = str.size();
        }
        words.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(" ", end + 1);
    }

    return errorHandler(words) ? std::make_pair(words[0], words[1])
                               : std::make_pair("", "");
}

bool SplitDialog::errorHandler(std::vector<std::string>& words)
{
    if (words.size() < 3) {
        return false;
    }

    const int lastIndex = words.size() - 1;
    int dashIndex{};
    while (dashIndex < lastIndex)
    {
        if (words[dashIndex] == "-" || words[dashIndex] == "–") {
            break;
        }
        ++dashIndex;
    }

    if (dashIndex == 0 || dashIndex == lastIndex) {
        return false;
    }

    std::string engWord = words[0];
    for (int i{1}; i < dashIndex; ++i) {
        engWord += " " + words[i];
    }

    std::string armWord = words[dashIndex + 1];
    for (int i{dashIndex + 2}; i <= lastIndex; ++i) {
        armWord += " " + words[i];
    }

    words[0] = engWord;
    words[1] = armWord;
    words.resize(2);

    return true;
}
