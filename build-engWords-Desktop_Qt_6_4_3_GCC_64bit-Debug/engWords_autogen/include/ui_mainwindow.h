/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QPushButton *newWord;
    QPushButton *pushButton_0;
    QSpacerItem *horizontalSpacer_1;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *clear;
    QLabel *countLabel;
    QSpacerItem *horizontalSpacer_4;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(550, 300);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        newWord = new QPushButton(centralwidget);
        newWord->setObjectName("newWord");
        newWord->setMinimumSize(QSize(0, 40));

        gridLayout_2->addWidget(newWord, 6, 2, 1, 2);

        pushButton_0 = new QPushButton(centralwidget);
        pushButton_0->setObjectName("pushButton_0");
        pushButton_0->setMinimumSize(QSize(200, 40));

        gridLayout_2->addWidget(pushButton_0, 3, 2, 1, 1);

        horizontalSpacer_1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_1, 5, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 3, 0, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);

        gridLayout_2->addWidget(label, 2, 0, 1, 5);

        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setMinimumSize(QSize(200, 40));

        gridLayout_2->addWidget(pushButton_3, 5, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 5, 4, 1, 1);

        pushButton_1 = new QPushButton(centralwidget);
        pushButton_1->setObjectName("pushButton_1");
        pushButton_1->setMinimumSize(QSize(200, 40));

        gridLayout_2->addWidget(pushButton_1, 3, 3, 1, 1);

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setMinimumSize(QSize(200, 40));

        gridLayout_2->addWidget(pushButton_2, 5, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 3, 4, 1, 1);

        clear = new QPushButton(centralwidget);
        clear->setObjectName("clear");
        clear->setMinimumSize(QSize(0, 40));
        clear->setMaximumSize(QSize(50, 16777215));
        clear->setLayoutDirection(Qt::LeftToRight);

        gridLayout_2->addWidget(clear, 0, 0, 1, 1);

        countLabel = new QLabel(centralwidget);
        countLabel->setObjectName("countLabel");
        countLabel->setMaximumSize(QSize(16777215, 20));
        countLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_2->addWidget(countLabel, 0, 1, 1, 3);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 4, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 550, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        newWord->setText(QCoreApplication::translate("MainWindow", "new word", nullptr));
        pushButton_0->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Get ready!", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        pushButton_1->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        clear->setText(QCoreApplication::translate("MainWindow", "clear", nullptr));
        countLabel->setText(QCoreApplication::translate("MainWindow", "0 of 0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
