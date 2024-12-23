#ifndef TEXTTOSPEECH_HPP
#define TEXTTOSPEECH_HPP

#include "define.hpp"

#ifdef _PLAY_SOUND_
#include <QWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFile>
#include <QUrl>
#include <QUrlQuery>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QDebug>

class TextToSpeech : public QWidget
{
    using baseClass = QWidget;
    Q_OBJECT

public:
    TextToSpeech(QWidget* parent = nullptr);
    ~TextToSpeech(){}

public slots:
    void fetchAudio(const QString& text);

private slots:
    void playAudio(QNetworkReply* reply);

private:
    QNetworkAccessManager*  m_manager;
    QMediaPlayer*           m_player;
    QAudioOutput*           m_audioOutput;
};
#endif // _PLAY_SOUND_

#endif // TEXTTOSPEECH_HPP
