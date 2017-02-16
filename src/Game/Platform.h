#ifndef PLATFORM_H
#define PLATFORM_H

#include "Core/GameObject.h"
#include "Core/Mesh.h"
#include <string>
#include <SDL.h>


class Texture;
class ShaderProgram;

class Platform : public GameObject, Mesh
{
	// The matrix is used to apply transformations when rendering with OpenGL
	glm::mat4 m_Matrix;

	// The texture reference is used to get the texture handle used to display this image
	Texture* m_Texture;

	float m_Width;
	float m_Height;

public:
	Platform();
	Platform(std::string &filePath);
	Platform(std::string &filePath, glm::vec3 position, glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), bool isEnabled = true);
	virtual ~Platform();

	// Loads the image through the texture class. It is called automatically if the file path is specified in the constructor
	virtual bool LoadTexture(std::string &filePath);

	// Sets the VBO and IBO for rendering this object based on the texture's dimensions
	virtual void SetupArrayBuffers();

	virtual void Update(double delta);
	virtual void Draw();
};

#endif PLATFORM_H