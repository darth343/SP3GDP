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
	cout << "StopOne" << endl;
}

#endif