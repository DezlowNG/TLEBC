/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifndef AMBIENCE_H
#define AMBIENCE_H

#include <irrklang/irrKlang.h>

namespace rb
{
	class Ambience
	{
		float mVolume = 0.5f;
		float mTempVolume = 0.0f;
	public:
		Ambience();
		~Ambience();

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