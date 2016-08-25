#include "irrKlang.h"
#include <string>

using namespace irrklang;
using std::string;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void Init();
	void SoundPlay(const char * fileName, ISound ** soundType, float volume, bool loop);
	void SoundPause(ISound ** soundType);
	void StopSingleSound(const char* fileName);
	void StopAllSound();



private:

	//Sound
	ISoundEngine* soundEngine;
	
};

