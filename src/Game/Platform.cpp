#include "Platform.h"

#include "Core/Texture.h"
#include <SDL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <SDL_image.h>
#include "Core/GameServices.h"

Platform::Platform() : GameObject(), Mesh(), m_Texture(nullptr), m_Width(0), m_Height(0)
{
	//
}

Platform::Platform(std::string &aFilePath) : GameObject(), Mesh(), m_Texture(nullptr), m_Width(0), m_Height(0)
{
	LoadTexture(aFilePath);
}

Platform::Platform(std::string &aFilePath, glm::vec3 aPosition, glm::vec3 aRotation, glm::vec3 aScale, bool aIsEnabled) : GameObject(aPosition, aRotation, aScale, aIsEnabled),
Mesh(), m_Texture(nullptr), m_Width(0), m_Height(0)
{
	LoadTexture(aFilePath);
}

Platform::~Platform()
{
	// The Texture class will delete the texture it loaded with OpenGL automatically when destroyed
	if (m_Texture != nullptr)
	{
		delete m_Texture;
		m_Texture = nullptr;
	}

	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

bool Platform::LoadTexture(std::string &aFilePath)
{
	if (m_Texture == nullptr)
	{
		m_Texture = new Texture();
		if (m_Texture->LoadTexture(aFilePath))
		{
			m_Width = 1000;
			m_Height = 1000;
			// If the width and height haven't been set yet, set them to the loaded texture's original dimensions
			if (m_Width == 0) { m_Width = (float)m_Texture->GetWidth(); }
			if (m_Height == 0) { m_Height = (float)m_Texture->GetHeight(); }

			SetupArrayBuffers();

			return true;
		}
	}

	return false;
}

void Platform::SetupArrayBuffers()
{
	/*
	// Set up the vertex buffer
	{
		// Generate a new vertex buffer if there isn't one already
		if (m_VBO == 0)
		{
			glGenBuffers(1, &m_VBO);
		}

		m_VertexCount = 4;

		// Define the vertices for a rectangle
		VertexFormat* vertices = new VertexFormat[m_VertexCount];
		//--			Position	--		UV		--		Normal	--
		vertices[0] = { 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 0, 0, 1 }; // Bottom left
		vertices[1] = { m_Width, 0.0f, 0.0f, 10.0f, 10.0f, 0, 0, 1 }; // Bottom right
		vertices[2] = { 0.0f, 0.0f, m_Height, 0.0f, 0.0f, 0, 0, 1 }; // Top left
		vertices[3] = { m_Width, 0.0f, m_Height, 10.0f, 0.0f, 0, 0, 1 }; // Top right
		//vertices[0].Position = { -0.8f, -0.8f, 0.0f };
		//vertices[1].Position = { 0.8f, -0.8f, 0.0f };
		//vertices[2].Position = { -0.8f, 0.8f, 0.0f };
		//vertices[3].Position = { 0.8f, 0.8f, 0.0f };
		// Note - SDL_Surface uses top-left as the origin, so the UVs must be flipped vertically

		// Bind the VBO to buffer the data into
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		// Buffer the vertex data
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_VertexCount, vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		delete[] vertices;
	}

	// Set up the index buffer
	{
		// Generate a new index buffer if there isn't one already
		if (m_IBO == 0)
		{
			glGenBuffers(1, &m_IBO);
		}

		m_IndexCount = 6;

		// Define the indices for a rectangle
		GLuint* indices = new GLuint[m_IndexCount] {0, 1, 2, 1, 3, 2};

		// Bind the VBO to buffer the data into
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

		// Buffer the vertex data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_IndexCount, indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		delete[] indices;
	}
	*/
}


void Platform::Update(double delta)
{
	//
}

void Platform::Draw()
{
	/*
	// Store the shader program handle
	GLuint shaderProgram = 0;// m_Shader->GetProgram();

	// Enable GL capabilities for this object to use to render
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	// Set the active shader to the one used by this object
	glUseProgram(shaderProgram);

	// Bind the vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	// Get the attribute and uniform locations from the shader program
	GLint aPos = glGetAttribLocation(shaderProgram, "a_Position");
	GLint aUV = glGetAttribLocation(shaderProgram, "a_UV");
    GLint aNormal = glGetAttribLocation(shaderProgram, "a_Normal");
	GLint uTex = glGetUniformLocation(shaderProgram, "u_Texture");
	GLint uWVP = glGetUniformLocation(shaderProgram, "u_WVP");

	// Set up attributes
	SDL_assert_release(aPos != -1);
	glEnableVertexAttribArray(aPos);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, Position));

	if (aUV != -1)
	{
		glEnableVertexAttribArray(aUV);
		glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, UV));
	}
    if (aNormal != -1)
	{
        glEnableVertexAttribArray(aNormal);
        glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, Normal));
	}

	// Set up uniforms
	if (uTex != -1)
	{
		// By default, textures are applied to the texture unit GL_TEXTURE0. This only needs to change if you are applying multiple textures to the same mesh
		glActiveTexture(GL_TEXTURE0 + 0);

		// Bind the current texture to be used by the shader
		glBindTexture(GL_TEXTURE_2D, m_Texture->GetTextureID());

		// Set u_Texture in the shader to use texture unit 0 (GL_TEXTURE0)
		glUniform1i(uTex, 0);
	}

	if (uWVP)
	{
		//TEMPORARY
		float x = 0.0015625f;
		float y = 0.0020833f;

		mat4 matrix = glm::translate(m_Position);

		matrix = GameServices::GetCameraManager()->GetActiveCamera()->GetViewProjectionMatrix() * matrix;

		glUniformMatrix4fv(uWVP, 1, false, &matrix[0][0]);
	}

	// Draw the object
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);


	// Disable attributes after using them
	glDisableVertexAttribArray(aPos);

	if (aUV != 0)
	{
		glDisableVertexAttribArray(aUV);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Disable the GL capabilities that this object uses
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	// Unbind this shader program
	glUseProgram(0);
	*/
}