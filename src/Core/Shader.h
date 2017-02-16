#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>


class RenderingManager;

//TODO: Upgrade to OpenGL 4+

class Shader
{
public:

	Shader();
	virtual ~Shader();

	//
	virtual bool Initialize() = 0;

	// Loads the shader from the files passed in, and compiles the shader program. Returns false if anything fails
	bool LoadShader(const char* vertShaderFile, const char* fragShaderFile);

	GLuint GetProgram();

	virtual void SetupAttributes();

	// Setup uniforms that are shared by every object rendering with this shader
	virtual void SetupSharedUniforms(RenderingManager* renderingManager);

	unsigned int GetRenderQueue();

	// Sets this as the active shader
	void UseShader();

	// Enables all OpenGL capabilities that this shader uses, such as blending
	// Should only be called by the RenderingManager when this shader is rendering objects
	virtual void EnableGLCapabilities();

	// Disables all OpenGL capabilities that this shader uses
	virtual void DisableGLCapabilities();


protected:
	char* m_VertShaderString;
	char* m_FragShaderString;

	GLuint m_VertShader;
	GLuint m_FragShader;
	GLuint m_Program;

	// When objects using this shader are rendered. Lower numbers render first
	unsigned int m_RenderQueue;
};



// A struct for defining vertex attributes in OpenGL.
struct VertexFormat
{
	// Specifies the vertex position
	glm::vec3 Position;

	// Specifies texture coordinates when rendering with a texture
	glm::vec2 UV;

	// Specifies the vertex normal
	glm::vec3 Normal;


	VertexFormat()
	{
		//
	}

	VertexFormat(float x, float y, float z, float u, float v, float nx, float ny, float nz)
	{
		Position = { x, y, z };
		UV = { u, v };
        Normal = { nx, ny, nz };
	}
	VertexFormat(glm::vec3 position, glm::vec2 uv = glm::vec2(0, 0), glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f))
    {
        Position = position;
        UV = uv;
        Normal = normal;
    }

    bool operator==(VertexFormat &other)
    {
        return (Position == other.Position && UV == other.UV && Normal == other.Normal);
    }
};



#endif //SHADER_H