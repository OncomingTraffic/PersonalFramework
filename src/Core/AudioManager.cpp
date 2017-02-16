#include "AudioManager.h"
#include <SDL.h>


AudioManager::AudioManager()
{
	int initFlags = MIX_INIT_MP3;
	if (Mix_Init(initFlags) != initFlags)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Mixer could not initialize! SDL_Mix Error: %s\n", Mix_GetError());
	}

	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Mix_OpenAudio failed! SDL_Mix Error: %s\n", Mix_GetError());
	}
}
AudioManager::~AudioManager()
{
	RemoveAllMusic();

	// Close all audio mixers
	while (Mix_QuerySpec(nullptr, nullptr, nullptr))
	{
		Mix_CloseAudio();
	}

	// Close SDL_mixer
	while (Mix_Init(0))
	{
		Mix_Quit();
	}
}

Mix_Chunk* AudioManager::AddSound(std::string aSoundFilePath)
{
	// Check if the sound is already loaded
	std::map<std::string, Mix_Chunk*>::iterator index = m_SoundEffects.find(aSoundFilePath);

	// If the sound was not found
	if (index == m_SoundEffects.end())
	{
		Mix_Chunk* sound = Mix_LoadWAV(aSoundFilePath.c_str());
		if (sound == nullptr)
		{
			// Sound was not opened
			return nullptr;
		}

		// Add the sound to the map
		m_SoundEffects.emplace(aSoundFilePath, sound);

		return sound;
	}
	else
	{
		// Return the map value at the found index
		return index->second;
	}
}

Mix_Music* AudioManager::AddMusic(std::string aMusicFilePath)
{
	// Check if the music is already loaded
	std::map<std::string, Mix_Music*>::iterator index = m_Music.find(aMusicFilePath);

	// If the music was not found
	if (index == m_Music.end())
	{
		Mix_Music* music = Mix_LoadMUS(aMusicFilePath.c_str());
		if (music == nullptr)
		{
			// Music was not opened
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Music could not be opened! SDL_Mix Error: %s\n", Mix_GetError());
			return nullptr;
		}

		// Add the music to the map
		m_Music.emplace(aMusicFilePath, music);

		return music;
	}
	else
	{
		// Return the map value at the found index
		return index->second;
	}
}

void AudioManager::RemoveSound(std::string aSoundFilePath)
{
	std::map<std::string, Mix_Chunk*>::iterator index = m_SoundEffects.find(aSoundFilePath);
	if (index != m_SoundEffects.end())
	{
		// TODO: Check that the chunk isn't playing
		Mix_FreeChunk(index->second);
		m_SoundEffects.erase(index);
	}
}

void AudioManager::RemoveMusic(std::string aMusicFilePath)
{
	std::map<std::string, Mix_Music*>::iterator index = m_Music.find(aMusicFilePath);
	if (index != m_Music.end())
	{
		// TODO: Check that the music isn't playing
		Mix_FreeMusic(index->second);
		m_Music.erase(index);
	}
}

void AudioManager::RemoveAllSounds()
{
	// Make sure no channels are playing audio
	if (Mix_Playing(-1))
	{
		Mix_HaltChannel(-1);
	}

	// Go through and close each sound in the map
	for (std::map<std::string, Mix_Chunk*>::iterator index = m_SoundEffects.begin(); index != m_SoundEffects.end(); ++index)
	{
		Mix_FreeChunk(index->second);
		m_SoundEffects.erase(index);
	}
}

void AudioManager::RemoveAllMusic()
{
	// Make sure music is not playing
	if (Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
	
	// Go through and close each music file in the map
	for (std::map<std::string, Mix_Music*>::iterator index = m_Music.begin(); index != m_Music.end();)
	{
		Mix_FreeMusic(index->second);
		index = m_Music.erase(index);
	}
}

void AudioManager::RemoveAll()
{
	RemoveAllSounds();
	RemoveAllMusic();
}