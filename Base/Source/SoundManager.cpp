#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "SoundManager.h"
#include <stddef.h>
#include <iostream>
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

void SoundManager::SoundPlay(const char*  fileName, ISound ** soundType)
{
	//Sounds
	if ((*soundType) == NULL)
		(*soundType) = soundEngine->play2D(fileName, false, true);
	if ((*soundType)->getIsPaused() == true)
		(*soundType)->setIsPaused(false);
	else if ((*soundType)->isFinished() == true)
		(*soundType) = NULL;
}

void SoundManager::SoundPause(ISound ** soundType)
{
	if ((*soundType) != NULL && (*soundType)->getIsPaused() == false)
		(*soundType)->setIsPaused(true);
	(*soundType) = NULL;
}

#endif