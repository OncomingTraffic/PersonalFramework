#include "ResourceManager.h"
#include <SDL.h>

#include "Texture.h"
#include "Mesh.h"
#include "ObjLoader.h"




ResourceManager::ResourceManager()
{
	//
}

ResourceManager::~ResourceManager()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Resource Manager closing");
}


bool ResourceManager::Initialize()
{
	//TODO: Load default resources

	bool success = true;

	if (LoadTextureFromFile(std::string("Textures/Default.bmp")) == nullptr)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Default texture could not load!");
		success = false;
	}


	return success;
}


template<> std::shared_ptr<Texture> ResourceManager::LoadResource(std::string aFilePath)
{
	// Check if the texture is already loaded
	std::unordered_map<std::string, std::shared_ptr<Texture>>::iterator index = m_Textures.find(std::string(aFilePath));

	// If the texture was not found
	if (index == m_Textures.end())
	{
		std::shared_ptr<Texture> texture = LoadTextureFromFile(aFilePath);
		if (texture == nullptr)
		{
			// Texture was not opened
			return nullptr;
		}

		// Add the texture to the map
		m_Textures.emplace(std::string(aFilePath), texture);

		return texture;
	}
	else
	{
		// Return the map value at the found index
		return index->second;
	}
}

template<> std::shared_ptr<Mesh> ResourceManager::LoadResource(std::string aFilePath)
{
	// Check if the mesh is already loaded
	std::unordered_map<std::string, std::shared_ptr<Mesh>>::iterator index = m_Meshes.find(std::string(aFilePath));

	// If the mesh was not found
	if (index == m_Meshes.end())
	{
		std::shared_ptr<Mesh> mesh = LoadMeshFromFile(aFilePath);
		if (mesh == nullptr)
		{
			// Mesh was not opened
			return nullptr;
		}

		// Add the mesh to the map
		m_Meshes.emplace(std::string(aFilePath), mesh);

		return mesh;
	}
	else
	{
		// Return the map value at the found index
		return index->second;
	}
}

template<> std::shared_ptr<Mix_Chunk> ResourceManager::LoadResource(std::string aFilePath)
{
	// Check if the sound is already loaded
	std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>>::iterator index = m_SoundEffects.find(aFilePath);

	// If the sound was not found
	if (index == m_SoundEffects.end())
	{
		std::shared_ptr<Mix_Chunk> sound(Mix_LoadWAV(aFilePath.c_str()), [&](Mix_Chunk* sound) // [&] will receive argument by reference, in this case the argument is 'aFilePath'
		{
			// Custom deleter lambda
			//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Closing Mix_Chunk: (%s)", aFilePath); // Cannot pass a local variable into a lambda
			Mix_FreeChunk(sound);
		});

		if (sound == NULL)
		{
			// Sound was not opened
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Sound could not be opened! SDL_Mix Error: %s\n", Mix_GetError());
			return nullptr;
		}

		// Add the sound to the map
		m_SoundEffects.emplace(aFilePath, sound);

		return sound;
	}
	else
	{
		// Return the map value at the found index
		return index->second;
	}
}

template<> std::shared_ptr<Mix_Music> ResourceManager::LoadResource(std::string aFilePath)
{
	// Check if the music is already loaded
	std::unordered_map<std::string, std::shared_ptr<Mix_Music>>::iterator index = m_Music.find(aFilePath);

	// If the music was not found
	if (index == m_Music.end())
	{
		std::shared_ptr<Mix_Music> music(Mix_LoadMUS(aFilePath.c_str()), [&](Mix_Music* music) // [&] will receive argument by reference, in this case the argument is 'aFilePath'
		{
			// Custom deleter lambda
			//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Closing Mix_Music: (%s)", "Test");
			Mix_FreeMusic(music);
		});


		if (music == NULL)
		{
			// Music was not opened
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Music could not be opened! SDL_Mix Error: %s\n", Mix_GetError());
			return nullptr;
		}

		// Add the music to the map
		m_Music.emplace(aFilePath, music);

		return music;
	}
	else
	{
		// Return the map value at the found index
		return index->second;
	}
}

template<> std::shared_ptr<TTF_Font> ResourceManager::LoadResource(std::string aFilePath)
{
	// Check if the font is already loaded
	std::unordered_map<std::string, std::shared_ptr<TTF_Font>>::iterator index = m_Fonts.find(std::string(aFilePath));

	// If the font was not found
	if (index == m_Fonts.end())
	{
		std::shared_ptr<TTF_Font> font = LoadFontFromFile(aFilePath, 12);
		if (font == nullptr)
		{
			// Font was not opened
			return nullptr;
		}

		// Add the font to the map
		m_Fonts.emplace(std::string(aFilePath), font);

		return font;
	}
	else
	{
		// Return the map value at the found index
		return index->second;
	}
}

template<> void ResourceManager::UnloadResource<Texture>(std::string &aFilePath)
{
	std::unordered_map<std::string, std::shared_ptr<Texture>>::iterator index = m_Textures.find(aFilePath);

	if (index != m_Textures.end())
	{
		if (!index->second.unique())
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Texture: %s is being referenced %i times\n", aFilePath, index->second.use_count());
			return;
		}

		m_Textures.erase(index);
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Texture not found in ResourceManager: (%s)\n", aFilePath);
	}
	
}

template<> void ResourceManager::UnloadResource<Mesh>(std::string &aFilePath)
{
	std::unordered_map<std::string, std::shared_ptr<Mesh>>::iterator index = m_Meshes.find(aFilePath);

	if (index != m_Meshes.end())
	{
		if (!index->second.unique())
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Mesh: %s is being referenced %i times\n", aFilePath, index->second.use_count());
			return;
		}

		m_Meshes.erase(index);
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Mesh not found in ResourceManager: (%s)\n", aFilePath);
	}
}

template<> void ResourceManager::UnloadResource<Mix_Chunk>(std::string &aFilePath)
{
	std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>>::iterator index = m_SoundEffects.find(aFilePath);

	if (index != m_SoundEffects.end())
	{
		if (!index->second.unique())
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Sound: %s is being referenced %i times\n", aFilePath, index->second.use_count());
			return;
		}

		m_SoundEffects.erase(index);
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Sound not found in ResourceManager: (%s)\n", aFilePath);
	}
}

template<> void ResourceManager::UnloadResource<Mix_Music>(std::string &aFilePath)
{
	std::unordered_map<std::string, std::shared_ptr<Mix_Music>>::iterator index = m_Music.find(aFilePath);

	if (index != m_Music.end())
	{
		if (!index->second.unique())
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Music: %s is being referenced %i times\n", aFilePath, index->second.use_count());
			return;
		}

		m_Music.erase(index);
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Music not found in ResourceManager: (%s)\n", aFilePath);
	}
}

template<> void ResourceManager::UnloadResource<TTF_Font>(std::string &aFilePath)
{
	std::unordered_map<std::string, std::shared_ptr<TTF_Font>>::iterator index = m_Fonts.find(aFilePath);

	if (index != m_Fonts.end())
	{
		if (!index->second.unique())
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Font: %s is being referenced %i times\n", aFilePath, index->second.use_count());
			return;
		}

		m_Fonts.erase(index);
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Font not found in ResourceManager: (%s)\n", aFilePath);
	}
}




std::shared_ptr<Texture> ResourceManager::LoadTextureFromFile(std::string &aFilePath)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	if (texture->LoadTexture(aFilePath))
	{
		return texture;
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Texture could not be loaded: File Path: (%s)\n", aFilePath);
		return nullptr;
	}
}

std::shared_ptr<Mesh> ResourceManager::LoadMeshFromFile(std::string &aFilePath)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	if (mesh->LoadMesh(aFilePath))
	{
		return mesh;
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Mesh could not be loaded: File Path: (%s)\n", aFilePath);
		return nullptr;
	}
}

std::shared_ptr<TTF_Font> ResourceManager::LoadFontFromFile(std::string &aFilePath, unsigned int aFontSize)
{
	//TODO: Is this a good way to handle deletion of fonts?
	std::shared_ptr<TTF_Font> font(TTF_OpenFont(aFilePath.c_str(), aFontSize), [&](TTF_Font* font) // [&] will receive argument by reference, in this case the argument is 'aFilePath'
	{
		// Custom deleter lambda
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Closing TTF_Font: (%s)", aFilePath);
		TTF_CloseFont(font);
	});

	if (font == NULL)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "TTF_Font could not be opened: Font Path(%s)\nError: %s\n", aFilePath, TTF_GetError());
		return nullptr;
	}
	return font;
}






//TODO:
//template<class T> T* ResourceManager::LoadResource()
//{
//	
//	SDL_assert(false);
//	//static_assert(true, "Undefined specialization");
//	return nullptr;
//}
//
//template<> ResourceManager* ResourceManager::LoadResource()
//{
//	//SDL_assert(false);
//	return nullptr;
//}