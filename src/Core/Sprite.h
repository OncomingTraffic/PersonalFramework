#ifndef SPRITE_H
#define SPRITE_H

#include "Core/GameObject.h"
#include "Core/Mesh.h"
#include <string>
#include <SDL.h>


class Texture;
class ShaderProgram;

class Sprite : public GameObject, Mesh
{
	// The matrix is used to apply transformations when rendering with OpenGL
	glm::mat4 m_Matrix;

	// The texture reference is used to get the texture handle used to display this image
	Texture* m_Texture;

	// Represents the center of the image, which determines where it is drawn relative to the center and around which point it rotates
	glm::vec2 m_AnchorPoint;

	float m_Width;
	float m_Height;


public:
	Sprite();
	Sprite(std::string &filePath);
	Sprite(std::string &filePath, glm::vec3 position, glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), bool isEnabled = true);
	virtual ~Sprite();

	// Loads the image through the texture class. It is called automatically if the file path is specified in the constructor
	virtual bool LoadTexture(std::string &filePath);

	// Sets the VBO and IBO for rendering this object based on the texture's dimensions
	virtual void SetupArrayBuffers();

	virtual void Update(double delta);
	virtual void Draw();

	virtual void SetAnchorPoint(glm::vec2 anchorPoint);
	virtual glm::vec2 GetAnchorPoint();

	virtual void SetWidth(float width);
	virtual float GetWidth();

	virtual void SetHeight(float height);
	virtual float GetHeight();
};


#endif SPRITE_H