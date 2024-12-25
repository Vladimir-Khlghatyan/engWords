#include "splitdialog.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

SplitDialog::SplitDialog(const QString& path, QWidget* parent)
    : baseClass(parent)
    , m_path(path)
{
    setWindowIcon(QIcon(":/icons/split.png"));
    setWindowTitle("Split file");
    setMinimumSize(400,100);

    QVBoxLayout* vLayout = new QVBoxLayout();
    QLabel* text = new QLabel("Choose a file in 'eng-arm' format for splitting.", this);
    text->setObjectName("SplitDialog");

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

    vLayout->addWidget(text, 0, Qt::AlignCenter);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
}

void SplitDialog::onBrows()
{
    m_filePath = QFileDialog::getOpenFileName(nullptr, "Select File", m_path, "*.txt");

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
}
