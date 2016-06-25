#pragma once
#include <string>
#include <memory>
#include <SDL_mixer.h>

using namespace std;

class ZeroEngine;
class Sound
	: public enable_shared_from_this<Sound>
{
public:
	bool isMusic;
	string name;
	Mix_Chunk* sound{ nullptr };
	Mix_Music* music{ nullptr };
	int channel = -1;

	Sound(std::weak_ptr<ZeroEngine> core_engine, string, Mix_Chunk*);
	Sound(std::weak_ptr<ZeroEngine> core_engine, string, Mix_Music*);
	void play(int = 0, int volume = 128);
	void pause();
	void stop();
	void changeVolume(int);
	~Sound();

	std::weak_ptr<ZeroEngine> core_engine;
};