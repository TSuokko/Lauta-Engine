#pragma once
#include <string>
#include "ECS.h"
#include "Components.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include <map>

enum SOUNDTYPE
{
	MUSIC,
	SOUNDEFFECT
};
//std::string sfxID, const char* path
class SoundComponent : public Component
{
public:

	SoundComponent() = default;

	SoundComponent(std::string ID, const char* path, SOUNDTYPE type)
	{
		switch (type)
		{
		case MUSIC:
			AddMusic(ID, path);
			break;
		case SOUNDEFFECT:
			AddSoundEffect(ID, path);
			break;
		default:
			printf("Error: incorrect sound type");
			break;
		}
		
	}

	//add sound to libraries
	void AddMusic(std::string mID, const char* path)
	{
		musicLibrary.emplace(mID, Mix_LoadMUS(path));
		//reservedChannel.emplace(mID, ++channels);
	}
	void AddSoundEffect(std::string sfxID, const char* path)
	{
		sfxLibrary.emplace(sfxID, Mix_LoadWAV(path));
		reservedChannel.emplace(sfxID, ++channels);
	}

	//get sound from libraries
	Mix_Music* GetMusic(std::string mID)
	{
		return musicLibrary[mID];
	}
	Mix_Chunk* GetSoundEffect(std::string sfxID)
	{
		return sfxLibrary[sfxID];
	}
	int getChannel(std::string sfxID)
	{
		return reservedChannel[sfxID];
	}


	void playMusic(std::string MusicID)
	{
		if (Mix_PlayingMusic() == 0)
			Mix_PlayMusic(GetMusic(MusicID), -1);
		else
			printf("Error: Another song is currently playing");
	}
	void pauseMusic()
	{
		Mix_PauseMusic();
	}
	void resumeMusic()
	{
		if (Mix_PausedMusic() == 1)
			Mix_ResumeMusic();	
	}
	void stopMusic()
	{
		Mix_HaltMusic();
	}

	void playSoundEffect(std::string sfxID)
	{	
		if (!Mix_Playing(getChannel(sfxID)))
		{
			int channel = Mix_PlayChannel(getChannel(sfxID), GetSoundEffect(sfxID), 0);
		}	
	}
	// 1 will loop the sound once,
	//-1 will loop the sound effect infinitely
	void playSoundEffectOnLoop(std::string sfxID, int numberOfLoops)
	{
		if (!Mix_Playing(getChannel(sfxID)))
		{
			Mix_PlayChannel(getChannel(sfxID), GetSoundEffect(sfxID), numberOfLoops);
		}
	}

private:

	std::map<std::string, Mix_Music*> musicLibrary;
	std::map<std::string, Mix_Chunk*> sfxLibrary;
	std::map<std::string, int> reservedChannel;

	bool playOnce = true;
	int channels = -1;

};

