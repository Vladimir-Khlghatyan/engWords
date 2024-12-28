#include "splitdialog.hpp"

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

    QVBoxLayout* vLayout = new QVBoxLayout();
    m_text = new QLabel("Choose a file in 'eng-arm' format for splitting.", this);
    m_text->setObjectName("SplitDialog");

    QHBoxLayout* hLayout = new QHBoxLayout();

    m_browsBtl = new QPushButton("", this);
    m_browsBtl->setIcon(QIcon(":/icons/source.png"));
    m_browsBtl->setFixedSize(QSize(30, 30));
    m_browsBtl->setIconSize(QSize(24, 24));
    m_browsBtl->setCursor(Qt::PointingHandCursor);
    m_browsBtl->setToolTip("Choose a file.");
    connect(m_browsBtl, &QPushButton::clicked, this, &SplitDialog::onBrows);

    m_cancelBtl = new QPushButton("Cancel", this);
    m_cancelBtl->setObjectName("SplitDialog");
    connect(m_cancelBtl, &QPushButton::clicked, this, &SplitDialog::onCencel);

    m_splitBtl = new QPushButton("Split", this);
    m_splitBtl->setObjectName("SplitDialog");
    m_splitBtl->setEnabled(false);
    connect(m_splitBtl, &QPushButton::clicked, this, &SplitDialog::onSplit);

    hLayout->addWidget(m_browsBtl, 0, Qt::AlignLeft);
    hLayout->addSpacerItem(new QSpacerItem(40,20));
    hLayout->addWidget(m_cancelBtl);
    hLayout->addWidget(m_splitBtl);

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
    m_splitBtl->setEnabled(!m_filePath.isEmpty());
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

    std::ofstream engFile(m_path.toStdString() + "/eng.txt", std::ios::app); // std::ios::app[end]
    std::ofstream armFile(m_path.toStdString() + "/arm.txt", std::ios::app); // std::ios::app[end]
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

    if (words.size() < 3 || words[1] != "-") {
        return {"",""};
    }

    for (int i{3}; i < words.size(); ++i) {
        words[2] += " " + words[i];
    }

    return {words[0], words[2]};
}
