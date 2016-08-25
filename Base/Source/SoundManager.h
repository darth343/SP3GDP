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
	void SoundPlay(const char * fileName, ISound ** soundType);
	void SoundPause(ISound ** soundType);

private:

	//Sound
	ISoundEngine* soundEngine;
	
};

