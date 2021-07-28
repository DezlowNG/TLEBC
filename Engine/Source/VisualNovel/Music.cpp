/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#include "Music.h"

namespace rb
{
	Music::Music()
	{
		SoundEngine = irrklang::createIrrKlangDevice();
	}

	Music::~Music()
	{
	}

	void Music::play(const char* path)
	{
		this->path = path;
		playMusic = true;
	}

	void Music::stop()
	{
		isStop = true;
	}

	void Music::SetVolume(float volume)
	{
		mVolume = volume;
		SoundEngine->setSoundVolume(volume);
		mTempVolume = volume;
	}

	void Music::update()
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