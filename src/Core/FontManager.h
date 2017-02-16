#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <map>
#include <string>
#include <SDL_ttf.h>

/*
A class used to handle SDL TrueType Fonts. It prevents the same font from being loaded twice.
TTF_Fonts are actually different if they use different font sizes, so a different font size will
create a new TTF_Font.
*/
class FontManager
{
	// A list of open fonts, mapped to a key pair of the font path and font size
	std::map<std::pair<std::string, unsigned int>, TTF_Font*> m_Fonts;

	// Opens the font through SDL_ttf, called automatically through the AddFont method
	TTF_Font* LoadFont(std::string &fontPath, unsigned int fontSize);

public:
	FontManager();
	~FontManager();

	// If the font already exists, return it, otherwise load the font then return it.
	TTF_Font* AddFont(std::string fontPath, unsigned int fontSize);

	// Close the font and remove from the map of loaded fonts
	void CloseFont(std::string fontPath, unsigned int fontSize);

	// Iterate through the map and close and remove all fonts
	void CloseAllFonts();
};


#endif FONTMANAGER_H