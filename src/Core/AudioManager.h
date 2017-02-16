#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL_mixer.h>
#include <string>
#include <map>

class AudioManager
{
	std::map<std::string, Mix_Chunk*> m_SoundEffects;

	std::map<std::string, Mix_Music*> m_Music;

	//void OpenAudio();

public:

	AudioManager();
	~AudioManager();

	// If the sound file already exists, return it, otherwise load the sound file then return it
	Mix_Chunk* AddSound(std::string soundFilePath);

	// If the music file already exists, return it, otherwise load the music file then return it
	Mix_Music* AddMusic(std::string musicFilePath);

	// Removes the sound from the list
	void RemoveSound(std::string soundFilePath);

	// Removes the music from the list, avoid removing the music that is currently playing
	void RemoveMusic(std::string musicFilePath);

	// Removes all sound file from the list
	void RemoveAllSounds();

	// Removes all music files from the list
	void RemoveAllMusic();

	// Unloads all music and sound files
	void RemoveAll();

};


#endif AUDIOMANAGER_H