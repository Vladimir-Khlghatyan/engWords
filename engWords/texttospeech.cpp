#include "texttospeech.hpp"

#ifdef _PLAY_SOUND_

TextToSpeech::TextToSpeech(QWidget* parent)
    : baseClass(parent)
    , m_manager(new QNetworkAccessManager(this))
    , m_player(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
{
    m_player->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(1.0);

    connect(m_manager, &QNetworkAccessManager::finished, this, &TextToSpeech::playAudio);
    connect(m_player, &QMediaPlayer::errorOccurred, this, &TextToSpeech::onError);
}

void TextToSpeech::fetchAudio(const QString& text)
{
    if (text.isEmpty()) {
        return;
    }

    QUrlQuery query;
    query.addQueryItem("ie", "UTF-8");
    query.addQueryItem("q", text);
    query.addQueryItem("tl", "en");
    query.addQueryItem("client", "tw-ob");

    QUrl url("https://translate.google.com/translate_tts");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");

    m_manager->get(request);
}

void TextToSpeech::playAudio(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        emit errorOccurred("Network error:   " + reply->errorString() + ".");
        reply->deleteLater();
        return;
    }

    QTemporaryFile tempFile;
    if (!tempFile.open())
    {
        emit errorOccurred("Failed to create temporary file.");
        reply->deleteLater();
        return;
    }

    QByteArray audioData = reply->readAll();
    tempFile.write(audioData);
    tempFile.close();

    QString filePath = tempFile.fileName();
    m_player->setSource(QUrl::fromLocalFile(filePath));
    m_player->play();
    reply->deleteLater();
}

void TextToSpeech::onError(QMediaPlayer::Error error)
{
    Q_UNUSED(error);
     emit errorOccurred(m_player->errorString() + "  Try again.");
//    emit errorOccurred("Failed to load audio. Try again.");
}

#endif // _PLAY_SOUND_
