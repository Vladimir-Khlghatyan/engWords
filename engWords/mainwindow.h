#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "define.hpp"
#include "texttospeech.hpp"

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QPushButton>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setUpButtonWithIcon(QPushButton* btn,
                             const QString& iconName,
                             const QString& tooltip = "",
                             bool isEnabled = true,
                             const QSize& buttonSize = QSize(30, 30),
                             const QSize& iconSize = QSize(24, 24));

private:
    QString     getExecutableGrandparentDirPath(void);
    QStringList readFromFile(const QString& filepath);
    int         getRandomNumber(int lower, int upper);
    void        writeToFileInAppendMode(const QString& filePath, const QStringList& source);
    void        writeToFileInTranscateMode(const QString& filePath, const QStringList& source);
    void        parse(QStringList& source);
    void        buttonPushAction(int index);

private slots:
    void onSourceButtonClicked();
    void onSoundPlayButtonClicked();
    void onNextButtonClicked();
    void onShowButtonClicked();
    void onResetButtonClicked();
    void onDeleteButtonClicked();
    void onSettingsButtonClicked();
    void onErrorMsg(const QString& msg);

private:
    Ui::MainWindow          *ui;
    QVector<QPushButton*>   m_button;
    QStringList             m_engWords;
    QStringList             m_armWords;
    QString                 m_sourcePath;
    int                     m_correctIndex;
    int                     m_totalAnswers;
    int                     m_rightAnswers;
    int                     m_deletedWords;
    int                     m_currentWordIndex;
    QTimer*                 m_dltTimer;
    QTimer*                 m_nextTimer;
    bool                    m_isEngArmMode;

#ifdef _PLAY_SOUND_
    TextToSpeech*           m_textToSpeech;
#endif // _PLAY_SOUND_

};
#endif // MAINWINDOW_H
