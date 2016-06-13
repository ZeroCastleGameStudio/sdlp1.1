#include <iostream>
#include "ZE_Core.h"
#include "ZE_Sound.h"

using namespace std;

Sound::Sound(string name, Mix_Music* muc)
{
	isMusic = true;
	this->name = name;
	music = muc;
}

Sound::Sound(string name, Mix_Chunk* chunk)
{
	isMusic = false;
	this->name = name;
	sound = chunk;
}

void Sound::play(int loop, int volume)
{
	if (isMusic)
	{
		Mix_PlayMusic(music, loop);
		Mix_VolumeMusic(volume);
	}
	else
	{
		Mix_VolumeChunk(sound, volume);
		channel = Mix_PlayChannel(-1, sound, loop);
	}
}

void Sound::changeVolume(int volume)
{
	if (isMusic)
		Mix_VolumeMusic(volume);
	else if (channel != -1)
		Mix_Volume(channel, volume);
}

void Sound::pause()
{
	if (isMusic)
		Mix_PauseMusic();
	else if(channel != -1)
		Mix_Pause(channel);
}

void Sound::stop()
{
	if (isMusic)
		Mix_HaltMusic();
	else if(channel != -1)
	{
		Mix_HaltChannel(channel);
		channel = -1;
	}
}

Sound::~Sound()
{
	if (isMusic)
	{
		/*可能是由于mixer自己的问题，也可能是由于MP3文件太傻比了。所以调用free的时候不能正确释放
		会弹出一个访问冲突错误。chunk的话就没有问题*/
		if (Mix_GetMusicType(music) == MUS_MP3)
		{
			ZE_error.PopDebutConsole_Warning("Can't free a mp3 music, SDL_Mixer is not support it");
		}
		else
			Mix_FreeMusic(music);
	}
	else
		Mix_FreeChunk(sound);
}