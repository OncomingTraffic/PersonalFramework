#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <SDL_mixer.h>
#include <SDL_ttf.h>



class Texture;
class Mesh;
//struct Mix_Chunk;
//struct Mix_Music;
//struct TTF_Font;


/*
Handles loading and destroying of all resources, such as models, audio, textures, etc.
Ensures that each resource will only be loaded once.
*/
class ResourceManager
{
public:

	ResourceManager();
	~ResourceManager();

	bool Initialize();


	// Load a resource and return it. Will ensure that a resource is only loaded once
	// Acceptable types are Texture, Mesh, Mix_Chunk, Mix_Music, TTF_Font
	template<class T> std::shared_ptr<T> LoadResource(std::string filePath);
	//template<> std::shared_ptr<Texture> ResourceManager::LoadResource(std::string filePath); // When defining explicit specializations for templates, you need to declare each specialization separately
	//template<> std::shared_ptr<Mesh> ResourceManager::LoadResource(std::string filePath);
	//template<> std::shared_ptr<Mix_Chunk> ResourceManager::LoadResource(std::string filePath);
	//template<> std::shared_ptr<Mix_Music> ResourceManager::LoadResource(std::string filePath);
	//template<> std::shared_ptr<TTF_Font> ResourceManager::LoadResource(std::string filePath);

	// Unload a resource using its file path as the key
	// Acceptable types are Texture, Mesh, Mix_Chunk, Mix_Music, TTF_Font
	template<class T> void UnloadResource(std::string &filePath);
	//template<Texture> void UnloadResource(std::string filePath);


protected:

	//TODO: Change all resources to inherit from a base resource class, which will help with deleting and logging resources
	//		Resources would possibly no longer be stored in unordered maps, since the resource class would store the resource name
	//		Not storing the file name makes them harder to hash or find

	std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
	std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Meshes;
	std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> m_SoundEffects;
	std::unordered_map<std::string, std::shared_ptr<Mix_Music>> m_Music;
	std::unordered_map<std::string, std::shared_ptr<TTF_Font>> m_Fonts; // Note - C++ does not have a hash funtion for std::pair, they cannot be used in unordered_maps

	std::shared_ptr<Texture> LoadTextureFromFile(std::string &filePath);
	std::shared_ptr<Mesh> LoadMeshFromFile(std::string &filePath);
	std::shared_ptr<TTF_Font> LoadFontFromFile(std::string &filePath, unsigned int fontSize);

	
};



#endif //RESOURCEMANAGER_H