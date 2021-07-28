/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifndef SOUND_H
#define SOUND_H

#include <irrklang/irrKlang.h>

namespace rb
{
	class Sound
	{
		float mVolume = 1.0f;
	public:
		Sound();
		~Sound() {}

		void play(const char* path);
		void stop();
		void SetVolume(float volume);

		irrklang::ISoundEngine* SoundEngine;
	};
}

#endif