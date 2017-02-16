#include "FileReader.h"

#include <SDL.h>

// Convert the contents of a file to a character array
char* LoadFile(const char* aFilePath)
{
	// Read the file to a SDL_RWops struct
	SDL_RWops* file = SDL_RWFromFile(aFilePath, "r");

	// Check to see if the file was read properly
	if (file == NULL)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(FileReader) File could not be opened: \"%s\"%s", aFilePath, " in function: \"char* LoadFile(const char* filePath)\"");
		return nullptr;
	}
	else
	{
		// Seek to the end of the file
		SDL_RWseek(file, 0, RW_SEEK_END);

		// Get the current offset (how far in the file we have seeked)
		const long size = SDL_RWtell(file);

		// Seek back to the beginning of the file
		SDL_RWseek(file, 0, RW_SEEK_SET);

		char* data;

		if (size > 0)
		{
			// Create a new char array that is one char larger than the file size
			data = new char[size + 1];

			// Read the file contents and store it in the char array "data", using "size" as the size will read the entire file
			SDL_RWread(file, data, size, 1);

			// Set the last element of the char array to a null character, so there are no issues reading the end-of-file
			data[size] = 0;

			// Close the SDL_RWops struct
			SDL_RWclose(file);

			return data;
		}
		else
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(FileReader) File could not be read: \"%s\"%s", aFilePath, " in function: \"char* LoadFile(const char* filePath)\"");
		}
	}

	return nullptr;
}
