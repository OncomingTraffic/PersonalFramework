#include "FontManager.h"

FontManager::FontManager()
{
	if (TTF_WasInit() == 0)
	{
		if (TTF_Init() == -1)
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		}
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "TTF was already enabled");
	}
}

FontManager::~FontManager()
{
	CloseAllFonts();
}

TTF_Font* FontManager::AddFont(std::string aFontPath, unsigned int aFontSize)
{
	// Check if the font is already loaded
	std::map<std::pair<std::string, unsigned int>, TTF_Font*>::iterator index = m_Fonts.find(std::pair<std::string, unsigned int>(aFontPath, aFontSize));

	// If the font was not found
	if (index == m_Fonts.end())
	{
		TTF_Font* font = LoadFont(aFontPath, aFontSize);
		if (font == nullptr)
		{
			// Font was not opened
			return nullptr;
		}

		// Add the font to the map
		m_Fonts.emplace(std::pair<std::string, unsigned int>(aFontPath, aFontSize), font);

		return font;
	}
	else
	{
		// Return the map value at the found index
		return index->second;
	}
}

void FontManager::CloseFont(std::string aFontPath, unsigned int aFontSize)
{
	std::map<std::pair<std::string, unsigned int>, TTF_Font*>::iterator index = m_Fonts.find(std::pair<std::string, unsigned int>(aFontPath, aFontSize));
	if (index != m_Fonts.end())
	{
		TTF_CloseFont(index->second);
		m_Fonts.erase(index);
	}
}

void FontManager::CloseAllFonts()
{
	// Go through and close each font in the map
	for (std::map<std::pair<std::string, unsigned int>, TTF_Font*>::iterator index = m_Fonts.begin(); index != m_Fonts.end();)
	{
		// index->first.first means the key of that element, which is a pair, and the key of the pair
		//CloseFont(index->first.first, index->first.second); // This looks stupid. TODO: Fix it
		//index = m_Fonts.begin();
		TTF_CloseFont(index->second);
		index = m_Fonts.erase(index);
	}
}

TTF_Font* FontManager::LoadFont(std::string &aFontPath, unsigned int aFontSize)
{
	TTF_Font* font = TTF_OpenFont(aFontPath.c_str(), aFontSize);
	if (font == NULL)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "TTF_Font could not be opened: Font Path(%s)\nError: %s\n", aFontPath.c_str(), TTF_GetError()); // Having a string and a char array seems to crash
		return nullptr;
	}
	return font;
}