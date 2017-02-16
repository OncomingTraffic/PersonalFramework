#include "Texture.h"

#include <SDL.h>
#include <SDL_image.h>
#include <GL\glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>


Texture::Texture() : m_TextureID(0), m_FileName(""), m_Width(0), m_Height(0)
{
	//
}

Texture::Texture(std::string &aFilePath) : m_TextureID(0), m_FileName(aFilePath), m_Width(0), m_Height(0)
{
	LoadTexture(m_FileName);
}

Texture::Texture(SDL_Surface* aSurface) : m_TextureID(0), m_Width(0), m_Height(0)
{
	LoadTexture(aSurface);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_TextureID);
	m_TextureID = 0;
}


bool Texture::LoadTexture(std::string &aFilePath)
{
	// Load the image using the SDL_Image load function, and store it in an SDL_Surface
	SDL_Surface* imageSurface = IMG_Load(aFilePath.c_str());

	if (!imageSurface)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Image was not successfully loaded. Error: %s\n", IMG_GetError());
		return false;
	}

	if (m_TextureID == 0)
	{
		// Generate a texture handle for this texture
		glGenTextures(1, &m_TextureID);
	}

	// Check the colour format of the surface
	GLenum colourFormat = GetSurfaceColourFormat(imageSurface);

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the pixel data for the texture, gets data and dimensions from the SDL_Surface
	glTexImage2D(GL_TEXTURE_2D, 0, colourFormat, imageSurface->w, imageSurface->h, 0, colourFormat, GL_UNSIGNED_BYTE, imageSurface->pixels);

	// Set the magnify and minify filter funtions
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Store the dimensions of the original image
	m_Width = imageSurface->w;
	m_Height = imageSurface->h;

	// Free the surface after using it
	SDL_FreeSurface(imageSurface);

	return true;
}


bool Texture::LoadTexture(SDL_Surface* aSurface, bool aFreeSurfaceAfterLoad)
{
	if (!aSurface)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Texture not loaded: Surface was null\n");
		return false;
	}

	if (m_TextureID == 0)
	{
		// Generate a texture handle for this texture
		glGenTextures(1, &m_TextureID);
	}

	// Check the colour format of the surface
	GLenum colourFormat = GetSurfaceColourFormat(aSurface);

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	// Set the pixel data for the texture, gets data and dimensions from the SDL_Surface
	glTexImage2D(GL_TEXTURE_2D, 0, colourFormat, aSurface->w, aSurface->h, 0, colourFormat, GL_UNSIGNED_BYTE, aSurface->pixels);

	// Set the magnify and minify filter funtions
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Store the dimensions of the original image
	m_Width = aSurface->w;
	m_Height = aSurface->h;

	if (aFreeSurfaceAfterLoad)
	{
		// Free the surface after using it
		SDL_FreeSurface(aSurface);
	}

	return true;
}

void Texture::DeleteTexture()
{
	glDeleteTextures(1, &m_TextureID);
	m_TextureID = 0;
}

unsigned int Texture::GetTextureID()
{
	return m_TextureID;
}

std::string Texture::GetFileName()
{
	return m_FileName;
}

unsigned int Texture::GetWidth()
{
	return m_Width;
}

unsigned int Texture::GetHeight()
{
	return m_Height;
}

// TODO: Change this function if any images fail to display
GLenum Texture::GetSurfaceColourFormat(SDL_Surface* surface)
{
	// get the number of channels in the SDL surface
	GLint colourChannels = surface->format->BytesPerPixel;
	if (colourChannels == 4) // Surface uses an alpha channel
	{
		/*if (surface->format->Rmask == 0x000000ff) // Check the endianness 
		{
			return GL_RGBA;
		}
		else
		{
			return GL_BGRA;
		}*/
		return GL_RGBA;
	}
	else if (colourChannels == 3) // Surface has no alpha channel
	{
		/*if (surface->format->Rmask == 0x000000ff)
		{
			return GL_RGB;
		}
		else
		{
			return GL_BGR;
		}*/
		return GL_RGB;
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Texture colour format could not be set: Surface is using an unkown format.");
		return 0;
	}
}