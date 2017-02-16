#ifndef CUBES_H
#define CUBES_H

#include "Core/GameObject.h"
#include <string>
#include <SDL.h>


class Texture;
class ShaderProgram;

class Cubes : public GameObject
{
	// The matrix is used to apply transformations when rendering with OpenGL
	glm::mat4 m_Matrix;

	// The texture reference is used to get the texture handle used to display this image
	Texture* m_Texture;

	// A handle for the vertex array buffer used by this object when rendering
	GLuint m_VBO;

	// A handle for the index array buffer used by this object when rendering
	GLuint m_IBO;

	// The number of vertices used to render this object
	unsigned int m_VertexCount;

	// The number of indices used to render this object
	unsigned int m_IndexCount;


public:
	Cubes();
	Cubes(glm::vec3 position, glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), bool isEnabled = true);
	virtual ~Cubes();

	// Sets the VBO and IBO for rendering this object based on the texture's dimensions
	virtual void SetupArrayBuffers();

	virtual void Update(double delta);
	virtual void Draw();
};


#endif CUBES_H