#pragma once
#include <string>
#include <SDL_Extension/SDL_mixer.h>

using namespace std;

class Sound
{
public:
	bool isMusic;
	string name;
	Mix_Chunk* sound;
	Mix_Music* music;
	int channel = -1;

	Sound(string, Mix_Chunk*);
	Sound(string, Mix_Music*);
	void play(int = 0, int volume = 128);
	void pause();
	void stop();
	void changeVolume(int);
	~Sound();
};