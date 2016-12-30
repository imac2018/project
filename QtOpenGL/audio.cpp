#include "audio.h"
#include "glexception.h"
#include <QMap>

QMap<QString, AudioPlayer*> AudioManager::library;

AudioManager::AudioManager()
{}

void AudioManager::addNewAudio(QString filename, QString name, bool loop)
{
	library.insert(name, new AudioPlayer(filename,name,loop));
}

void AudioManager::initAudioLibrary()
{

}

AudioPlayer *AudioManager::player(QString name)
{
	return library.value(name,NULL);
}


AudioPlayer::AudioPlayer(QString filename, QString key, bool loop)
	: key(key), loop(loop)
{
	file.setFileName(filename);

	file.open(QIODevice::ReadOnly);

	QAudioFormat format;
	// Set up the format, eg.
	format.setSampleRate(8000);
	format.setChannelCount(1);
	format.setSampleSize(8);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	if (!info.isFormatSupported(format)) {
		throw GLException("Audio Player Error", "The audio device can't play the required sound");
	}

	audio = new QAudioOutput(format, this);
	audio->setVolume(0.5);
	connect(audio, SIGNAL(stateChanged(QAudio::State)), this,
			SLOT(handleStateChanged(QAudio::State)));
}

void AudioPlayer::play()
{
	if(file.isOpen())
		audio->start(&file);
	else
		restart();
}

void AudioPlayer::stop()
{
	audio->stop();
}

void AudioPlayer::restart()
{
	audio->stop();
	audio->reset();
	file.close();
	file.flush();
	file.setFileName(file.fileName());
	file.open(QIODevice::ReadOnly);
	if(file.isOpen())
		audio->start(&file);
}

void AudioPlayer::setVolume(qreal volume)
{
	audio->setVolume(volume);
}

void AudioPlayer::handleStateChanged(QAudio::State newState)
{
	switch (newState) {
		case QAudio::IdleState:
			// Finished playing (no more data)
			audio->stop();
			file.close();
			if(loop){
				restart();
			}
			break;

		case QAudio::StoppedState:
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
