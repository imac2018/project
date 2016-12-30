#ifndef AUDIO_H
#define AUDIO_H

#include <QAudio>
#include <QAudioOutput>
#include <QFile>
#include <QMap>

class AudioManager;
class AudioPlayer : QObject{

	Q_OBJECT

	QAudioOutput* audio;
	QFile file;
	QString key;
	bool loop;
public:
	AudioPlayer(QString filename, QString key, bool loop = false);
	void play();
	void stop();
	void restart();
	void setVolume(qreal volume);

public slots:
	void handleStateChanged(QAudio::State);
};
class AudioManager
{
	friend class AudioPlayer;

	static QMap<QString, AudioPlayer*> library;

	static void addNewAudio(QString filename, QString name, bool loop);
public:
	AudioManager();
	static void initAudioLibrary();
	AudioPlayer* player(QString name);
};

#endif // AUDIO_H
