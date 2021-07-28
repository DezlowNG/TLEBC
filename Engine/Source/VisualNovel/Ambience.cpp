/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "Ambience.h"

namespace rb
{
	Ambience::Ambience()
	{
		SoundEngine = irrklang::createIrrKlangDevice();
	}

	Ambience::~Ambience()
	{
	}

	void Ambience::play(const char* path)
	{
		this->path = path;
		playMusic = true;
	}

	void Ambience::stop()
	{
		isStop = true;
	}

	void Ambience::SetVolume(float volume)
	{
		mVolume = volume;
		SoundEngine->setSoundVolume(volume);
		mTempVolume = volume;
	}

	void Ambience::update()
	{
		if (SoundEngine->getSoundVolume() < mVolume && isPlaying && !isStop || (playMusic && !isStop))
		{
			if (playMusic && !isStop)
			{
				mTempVolume = 0.0f;
				SoundEngine->setSoundVolume(0);
				SoundEngine->play2D(path.c_str(), true);
				playMusic = false;
				isPlaying = true;
			}
			SoundEngine->setSoundVolume(mTempVolume);
			mTempVolume += 0.001f;
		}

		if (isStop && isPlaying)
		{
			if (mTempVolume < 0.0f)
			{
				SoundEngine->stopAllSounds();
				isPlaying = false;
				isStop = false;
				return;
			}
			SoundEngine->setSoundVolume(mTempVolume);
			mTempVolume -= 0.001f;
		}
	}
}