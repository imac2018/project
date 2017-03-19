#include "audio.h"
#include "glexception.h"
#include <QMap>
#include <QDir>
#include <QMessageBox>

QMap<QString, AudioPlayer*> AudioManager::library;

AudioManager::AudioManager()
{}

void AudioManager::addNewAudio(QString filename, QString name, bool loop)
{
	library.insert(name, new AudioPlayer(filename,name,loop));
}

void AudioManager::initAudioLibrary()
{
	QDir soundDir("assets/sounds");
	if(!soundDir.exists()){
		QMessageBox::warning(NULL,"Audio Import Warning","The audio ressources directory has not been found\n"
														 "You can play but there will be no sound",QMessageBox::Ok);
	}
	else{
		QStringList filters;
		filters << "*.mp3" << "*.wav" << "*.ogg";
		QStringList loopFiles;
		loopFiles << "apu.mp3" << "generique.mp3";
		foreach(QString filename, soundDir.entryList(filters)){
            addNewAudio(soundDir.absoluteFilePath(filename),filename,loopFiles.contains(filename));
		}
	}
}

AudioPlayer *AudioManager::player(QString name)
{
	return library.value(name,NULL);
}


AudioPlayer::AudioPlayer(QString filename, QString key, bool loop)
	: key(key), loop(loop)
{
	audio = new QMediaPlayer(this);
    audio->setMedia(QMediaContent(QUrl::fromLocalFile(filename)));
	connect(audio, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this,
			SLOT(handleStateChanged(QMediaPlayer::MediaStatus)));
}

void AudioPlayer::play()
{
	audio->play();
}

void AudioPlayer::stop()
{
	audio->stop();
}

void AudioPlayer::pause()
{
	audio->pause();
}

void AudioPlayer::restart()
{
	audio->stop();
	audio->play();
}

void AudioPlayer::setVolume(int volume)
{
	audio->setVolume(volume);
}

void AudioPlayer::handleStateChanged(QMediaPlayer::MediaStatus newState)
{
	switch (newState) {
		case QMediaPlayer::EndOfMedia:
			// Finished playing (no more data)
			if(loop){
				restart();
			}
			break;

		case QMediaPlayer::InvalidMedia:
			// Stopped for other reasons
			if (audio->error() != QAudio::NoError) {
				switch(audio->error()){
				case QAudio::OpenError:
					throw GLException("Audio Player Error",
									  key+": An error occurred opening the audio device");
				case QAudio::IOError:
					throw GLException("Audio Player Error",
									  key+": An error occurred during read/write of audio device");
				case QAudio::UnderrunError:
					throw GLException("Audio Player Error",
									  key+": Audio data is not being fed to the audio"
										  " device at a fast enough rate");
				default:
					break;
				}
			}
			break;

		default:
			// ... other cases as appropriate
			break;
	}
}
