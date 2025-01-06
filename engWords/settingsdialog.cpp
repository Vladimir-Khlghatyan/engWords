#include "settingsdialog.hpp"
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

SettingsDialog::SettingsDialog(bool& isEngArmMode, bool& showPossAnsImmediately, QWidget* parent)
    : baseClass(parent)
    , m_isEngArmMode(isEngArmMode)
    , m_parentEngArmMode(isEngArmMode)
    , m_autoSlide(showPossAnsImmediately)
    , m_parentAutoSlide(showPossAnsImmediately)
{
    setWindowIcon(QIcon(":/icons/settings.png"));
    setWindowTitle("Settings");
    setFixedSize(320,100);

    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent);

    m_swapBtn = new QPushButton("", this);
    mainWindow->setUpButtonWithIcon(m_swapBtn, m_isEngArmMode ? "toggle_off" : "toggle_on",
                                    "Swap languages", true, QSize(30, 20), QSize(30, 30));
    m_swapBtn->setStyleSheet("QPushButton { background-color: #2e2f30; }");
    connect(m_swapBtn, &QPushButton::clicked, this, &SettingsDialog::onSwap);

    m_swapText = new QLabel("", this);
    m_swapText->setText(m_isEngArmMode ? "Eng - Arm" : "Arm - Eng");
    m_swapText->setObjectName("dialog");

    m_autoSlieBtn = new QPushButton("", this);
    mainWindow->setUpButtonWithIcon(m_autoSlieBtn, m_autoSlide ? "toggle_on" : "toggle_off",
                                    "Show possible answers when the Next button is clicked.", true, QSize(30, 20), QSize(30, 30));
    m_autoSlieBtn->setStyleSheet("QPushButton { background-color: #2e2f30; }");
    connect(m_autoSlieBtn, &QPushButton::clicked, this, &SettingsDialog::onAutoSlie);

    m_autoSlieText = new QLabel("", this);
    m_autoSlieText->setText(m_autoSlide ? "Auto Slide: ON" : "Auto Slide: OFF");
    m_autoSlieText->setObjectName("dialog");

    m_saveBtn = new QPushButton("Save", this);
    m_saveBtn->setCursor(Qt::PointingHandCursor);
    m_saveBtn->setObjectName("dialog");
    connect(m_saveBtn, &QPushButton::clicked, this, &SettingsDialog::onSave);

    m_cancelBtn = new QPushButton("Cancel", this);
    m_cancelBtn->setCursor(Qt::PointingHandCursor);
    m_cancelBtn->setObjectName("dialog");
    connect(m_cancelBtn, &QPushButton::clicked, this, &SettingsDialog::onCencel);

    QHBoxLayout* hLayout0 = new QHBoxLayout();
    hLayout0->addWidget(m_swapBtn);
    hLayout0->addWidget(m_swapText, 0, Qt::AlignLeft);

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(m_autoSlieBtn);
    hLayout1->addWidget(m_autoSlieText, 0, Qt::AlignLeft);

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addSpacerItem(new QSpacerItem(40,20));
    hLayout2->addWidget(m_cancelBtn, 0, Qt::AlignRight);
    hLayout2->addWidget(m_saveBtn);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout0);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);

    setLayout(vLayout);
}

void SettingsDialog::onSwap()
{
    m_isEngArmMode = !m_isEngArmMode;
    m_swapBtn->setIcon(QIcon(m_isEngArmMode ? ":/icons/toggle_off.png" : ":/icons/toggle_on.png"));
    m_swapText->setText(m_isEngArmMode ? "Eng - Arm" : "Arm - Eng");
}

void SettingsDialog::onAutoSlie()
{
    m_autoSlide = !m_autoSlide;
    m_autoSlieBtn->setIcon(QIcon(m_autoSlide ? ":/icons/toggle_on.png" : ":/icons/toggle_off.png"));
    m_autoSlieText->setText(m_autoSlide ? "Auto Slide: ON" : "Auto Slide: OFF");
}

void SettingsDialog::onCencel()
{
    reject();
}

void SettingsDialog::onSave()
{
    m_parentAutoSlide = m_autoSlide;

    if (m_parentEngArmMode == m_isEngArmMode) {
        reject();
    } else {
        m_parentEngArmMode = m_isEngArmMode;
        accept();
    }
}
