#include "settingsdialog.hpp"
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

SettingsDialog::SettingsDialog(QWidget* parent)
    : baseClass(parent)
    , m_isEngArmMode(true)
{
    setWindowIcon(QIcon(":/icons/settings.png"));
    setWindowTitle("Settings");
    setMinimumSize(300,100);

    m_toggleBtn = new QPushButton("", this);
    qobject_cast<MainWindow*>(parent)->setUpButtonWithIcon(m_toggleBtn,
                                    m_isEngArmMode ? "toggle_eng" : "toggle_arm",
                                    "Swap languages", true, QSize(30, 20), QSize(30, 30));
    m_toggleBtn->setStyleSheet("QPushButton { background-color: #2e2f30; }");
    connect(m_toggleBtn, &QPushButton::clicked, this, &SettingsDialog::onToggle);

    m_toggleText = new QLabel(m_isEngArmMode ? "Eng - Arm" : "Arm - Eng", this);
    m_toggleText->setStyleSheet("QLabel { font-size: 14px; }");
    m_toggleText->setObjectName("dialog");

    m_saveBtn = new QPushButton("Save", this);
    m_saveBtn->setCursor(Qt::PointingHandCursor);
    m_saveBtn->setObjectName("dialog");
    connect(m_saveBtn, &QPushButton::clicked, this, &SettingsDialog::onSave);

    m_cancelBtn = new QPushButton("Cancel", this);
    m_cancelBtn->setCursor(Qt::PointingHandCursor);
    m_cancelBtn->setObjectName("dialog");
    connect(m_cancelBtn, &QPushButton::clicked, this, &SettingsDialog::onCencel);

    QHBoxLayout* hLayout0 = new QHBoxLayout();
    hLayout0->addWidget(m_toggleBtn);
    hLayout0->addWidget(m_toggleText, 0, Qt::AlignLeft);
//    hLayout0->addSpacerItem(new QSpacerItem(40,20));

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addSpacerItem(new QSpacerItem(40,20));
    hLayout1->addWidget(m_cancelBtn, 0, Qt::AlignRight);
    hLayout1->addWidget(m_saveBtn);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout0);
    vLayout->addLayout(hLayout1);

    setLayout(vLayout);
}

void SettingsDialog::onToggle()
{
    m_isEngArmMode = !m_isEngArmMode;
    m_toggleBtn->setIcon(QIcon(m_isEngArmMode ? ":/icons/toggle_eng.png" : ":/icons/toggle_arm.png"));
    m_toggleText->setText(m_isEngArmMode ? "Eng - Arm" : "Arm - Eng");
}

void SettingsDialog::onCencel()
{
    reject();
}

void SettingsDialog::onSave()
{
    accept();
}
