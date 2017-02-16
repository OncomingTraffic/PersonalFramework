#ifndef LABEL_H
#define LABEL_H

#include "Core/GameObject.h"
#include "Core/Mesh.h"
#include <string>
#include <SDL_ttf.h>

class Texture;

/*
A class that displays 2D text
*/
class Label : public GameObject, Mesh
{
	// The text that this label displays
	std::string m_Text;

	// The font that the text displays in
	TTF_Font* m_Font;

	// How large the font is
	unsigned int m_FontSize;

	// What color the font is drawn in
	SDL_Color m_FontColor;

	// The matrix is used to apply transformations when rendering with OpenGL
	glm::mat4 m_Matrix;

	// The texture reference is used to get the texture handle used to display this label
	Texture* m_Texture;

	// Represents the center of the image, which determines where it is drawn relative to the center and around which point it rotates
	glm::vec2 m_AnchorPoint;

	float m_Width;
	float m_Height;

public:
	Label(std::string &text, std::string &fontPath, unsigned int fontSize = 24, SDL_Color fontColor = SDL_Color{ 0, 0, 0, 255 }, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), bool isEnabled = true);
	virtual ~Label();

	// 
	virtual bool LoadFont(std::string &fontPath);

	// Loads the image through the texture class
	virtual bool LoadTexture();

	// Sets the VBO and IBO for rendering this object based on the texture's dimensions
	virtual void SetupArrayBuffers();

	virtual void Update(double delta);
	virtual void Draw();

	virtual void SetText(std::string text);
};



#endif LABEL_H