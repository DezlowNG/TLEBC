/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifndef MUSIC_H
#define MUSIC_H

#include <irrklang/irrKlang.h>

namespace rb
{
	class Music
	{
		float mVolume = 0.5f;
		float mTempVolume = 0.0f;
	public:
		Music();
		~Music();

		void play(const char* path);
		void stop();
		void SetVolume(float volume);
		void update();

		irrklang::ISoundEngine* SoundEngine;
		bool isStop = false;
		bool isPlaying = false;
		bool playMusic = false;

		std::string path;
	};
}

#endif