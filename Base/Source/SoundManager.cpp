#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "SoundManager.h"
#include <stddef.h>
#include <iostream>

using std::cout;
using std::endl;

SoundManager::SoundManager()
	: soundEngine(NULL)
{
	soundEngine = irrklang::createIrrKlangDevice();
}

SoundManager::~SoundManager()
{
	if (soundEngine)
	{
		soundEngine->drop();
	}
}

void SoundManager::Init()
{
	// Sound
	soundEngine = createIrrKlangDevice();
	if (!soundEngine)
		return;
}

void SoundManager::SoundPlay(const char*  fileName, ISound ** soundType, float volume, bool loop)
{
	//Sounds
	if ((*soundType) == NULL)
		(*soundType) = soundEngine->play2D(fileName, loop, true);
	if ((*soundType)->getIsPaused() == true)
		(*soundType)->setIsPaused(false);
	else if ((*soundType)->isFinished() == true)
		(*soundType) = NULL;
	if ((*soundType))
		(*soundType)->setVolume(volume);
}
void SoundManager::playSE(string Music)
{
	SoundEffect = soundEngine->play2D(Music.c_str(), false, false, false);
}
void SoundManager::playMusic(string Music)
{
	Song = soundEngine->play2D(Music.c_str(), true, false, true);
	//Song->setVolume(0.5);

}
void SoundManager::stopMusic(string Music)
{
	Song->stop();
}
void SoundManager::SoundPause(ISound ** soundType)
{
	if ((*soundType) != NULL && (*soundType)->getIsPaused() == false)
		(*soundType)->setIsPaused(true);
	(*soundType) = NULL;
}

void SoundManager::StopAllSound()
{
	soundEngine->removeAllSoundSources();
}

void SoundManager::StopSingleSound(const char* fileName)
{
	soundEngine->removeSoundSource(fileName);
}

#endif