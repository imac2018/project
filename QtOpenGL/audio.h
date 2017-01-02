#ifndef AUDIO_H
#define AUDIO_H

#include <QAudio>
#include <QFile>
#include <QMap>
#include <QMediaPlayer>

class AudioManager;
class AudioPlayer : QObject{

	Q_OBJECT

	QString key;
	bool loop;
	QMediaPlayer* audio;
public:

	AudioPlayer(QString filename, QString key, bool loop = false);
	void play();
	void stop();
	void pause();
	void restart();
	void setVolume(int volume);

public slots:
	void handleStateChanged(QMediaPlayer::MediaStatus);
};
class AudioManager
{
	friend class AudioPlayer;

	static QMap<QString, AudioPlayer*> library;

	static void addNewAudio(QString filename, QString name, bool loop);
public:
	AudioManager();
	static void initAudioLibrary();
	static AudioPlayer* player(QString name);
};

#endif // AUDIO_H
