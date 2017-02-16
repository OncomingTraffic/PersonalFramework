#include "Cubes.h"

#include <SDL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <SDL_image.h>
#include "Core/Sprite.h"
#include "Core/Texture.h"
#include "Core/GameServices.h"

Cubes::Cubes() : GameObject(), m_Texture(nullptr), m_VBO(0), m_IBO(0), m_VertexCount(0), m_IndexCount(0)
{
	SetupArrayBuffers();
}

Cubes::Cubes(glm::vec3 aPosition, glm::vec3 aRotation, glm::vec3 aScale, bool aIsEnabled) : GameObject(aPosition, aRotation, aScale, aIsEnabled),
m_VBO(0), m_IBO(0), m_VertexCount(0), m_IndexCount(0)
{
	SetupArrayBuffers();
}

Cubes::~Cubes()
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


void Cubes::SetupArrayBuffers()
{
	/*
	// Set up the vertex buffer
	{
		// Generate a new vertex buffer if there isn't one already
		if (m_VBO == 0)
		{
			glGenBuffers(1, &m_VBO);
		}
		
		m_VertexCount = 64;

		// Define the vertices for a rectangle
		VertexFormat* vertices = new VertexFormat[m_VertexCount];
		for (int z = 0; z < 2; ++z)
		{
			for (int y = 0; y < 2; ++y)
			{
				for (int x = 0; x < 2; ++x)
				{
					float offset = 100.0f;
					int num = x + y * 2 + z * 4;
					//--					Position	--												UV		--		Color	--
					vertices[0 + 8 * num] = { -25.0f + offset * x, -25.0f + offset * y, 25.0f - offset * z, 0.0f, 1.0f, 0, 1.0f, 1.0f }; // Bottom left  2  3
					vertices[1 + 8 * num] = { 25.0f + offset * x, -25.0f + offset * y, 25.0f - offset * z, 1.0f, 1.0f, 1.0f, 0, 1.0f}; // Bottom right  0  1
					vertices[2 + 8 * num] = { -25.0f + offset * x, 25.0f + offset * y, 25.0f - offset * z, 0.0f, 0.0f, 1.0f, 1.0f, 0 }; // Top left
					vertices[3 + 8 * num] = { 25.0f + offset * x, 25.0f + offset * y, 25.0f - offset * z, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f }; // Top right

					vertices[4 + 8 * num] = { -25.0f + offset * x, -25.0f + offset * y, -25.0f - offset * z, 0.0f, 1.0f, 0, 0, 1.0f }; // Bottom left  6  7
					vertices[5 + 8 * num] = { 25.0f + offset * x, -25.0f + offset * y, -25.0f - offset * z, 1.0f, 1.0f, 1.0f, 0, 0 }; // Bottom right  4  5
					vertices[6 + 8 * num] = { -25.0f + offset * x, 25.0f + offset * y, -25.0f - offset * z, 0.0f, 0.0f, 0, 1.0f, 0 }; // Top left
					vertices[7 + 8 * num] = { 25.0f + offset * x, 25.0f + offset * y, -25.0f - offset * z, 1.0f, 0.0f, 0, 0, 0 }; // Top right
				}
			}
		}
		/
		m_VertexCount = 8;
		VertexFormat* vertices = new VertexFormat[m_VertexCount];
		vertices[0] = { -25.0f, -25.0f, 25.0f, 0.0f, 1.0f, 0, 1.0f, 1.0f, 1.0f }; // Bottom left  2  3
		vertices[1] = { 25.0f, -25.0f, 25.0f, 1.0f, 1.0f, 1.0f, 0, 1.0f, 1.0f }; // Bottom right  0  1
		vertices[2] = { -25.0f, 25.0f, 25.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0, 1.0f }; // Top left
		vertices[3] = { 25.0f, 25.0f, 25.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }; // Top right

		vertices[4] = { -25.0f, -25.0f, -25.0f, 0.0f, 1.0f, 0, 0, 1.0f, 1.0f }; // Bottom left  6  7
		vertices[5] = { 25.0f, -25.0f, -25.0f, 1.0f, 1.0f, 1.0f, 0, 0, 1.0f }; // Bottom right  4  5
		vertices[6] = { -25.0f, 25.0f, -25.0f, 0.0f, 0.0f, 0, 1.0f, 0, 1.0f }; // Top left
		vertices[7] = { 25.0f, 25.0f, -25.0f, 1.0f, 0.0f, 0, 0, 0, 1.0f }; // Top right
		/
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

		m_IndexCount = 36 * 8;

		// Define the indices for a rectangle
		GLuint* indices = new GLuint[m_IndexCount];

		for (int i = 0; i < 8; ++i)
		{
			int num = i * 36;
			int num2 = i * 8;
			indices[0 + num] = 0 + num2; //Front
			indices[1 + num] = 1 + num2;
			indices[2 + num] = 2 + num2;
			indices[3 + num] = 1 + num2;
			indices[4 + num] = 3 + num2;
			indices[5 + num] = 2 + num2;

			indices[6 + num] = 2 + num2; //Top
			indices[7 + num] = 3 + num2;
			indices[8 + num] = 6 + num2;
			indices[9 + num] = 3 + num2;
			indices[10 + num] = 7 + num2;//2 3    6 7
			indices[11 + num] = 6 + num2;//0 1    4 5

			indices[12 + num] = 6 + num2; //Back
			indices[13 + num] = 7 + num2;
			indices[14 + num] = 4 + num2;
			indices[15 + num] = 7 + num2;
			indices[16 + num] = 5 + num2;//2 3    6 7
			indices[17 + num] = 4 + num2;//0 1    4 5 

			indices[18 + num] = 4 + num2; //Bottom
			indices[19 + num] = 5 + num2;
			indices[20 + num] = 0 + num2;
			indices[21 + num] = 5 + num2;
			indices[22 + num] = 1 + num2;//2 3    6 7
			indices[23 + num] = 0 + num2;//0 1    4 5 

			indices[24 + num] = 4 + num2; //Left
			indices[25 + num] = 0 + num2;
			indices[26 + num] = 6 + num2;
			indices[27 + num] = 0 + num2;
			indices[28 + num] = 2 + num2;//6 2
			indices[29 + num] = 6 + num2;//4 0

			indices[30 + num] = 1 + num2; //Right
			indices[31 + num] = 5 + num2;
			indices[32 + num] = 3 + num2;
			indices[33 + num] = 5 + num2;
			indices[34 + num] = 7 + num2;//3 7
			indices[35 + num] = 3 + num2;//1 5
		}
		/m_IndexCount = 36;
		GLuint* indices = new GLuint[m_IndexCount];
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 1;
		indices[4] = 3;
		indices[5] = 2;/

		// Bind the VBO to buffer the data into
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

		// Buffer the vertex data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_IndexCount, indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		delete[] indices;
	}
	*/
}


void Cubes::Update(double delta)
{
	//
}

void Cubes::Draw()
{
	/*
	// Store the shader program handle
	GLuint shaderProgram = 0;// m_Shader->GetProgram();
	
	// Set the active shader to the one used by this object
	glUseProgram(shaderProgram);

	// Enable GL capabilities for this object to use to render
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

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

	if (uWVP != -1)
	{
		//TEMPORARY
		float x = 0.0015625f;
		float y = 0.0020833f;

		mat4 view = glm::lookAt(glm::vec3(50, 100, 300), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		mat4 matrix = {
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, y, 0,
			0, 0, 0, 1
		};
		//glm::rotate(matrix, 3.14159f / 5.f, vec3(1.0f, 0.0f, 0.0f));
		//matrix = glm::rotate(matrix, 3.14159f / 5.f, glm::vec3(1.0f, 1.0f, 1.0f));
		//matrix = glm::perspective(45.0f, 640.0f / 640.0f, 0.01f, 100.0f)* matrix * view * * glm::rotate(3.14159f * 0.5f, vec3(1.0f, 0.0f, 0.0f))/;
		//matrix = glm::perspectiveFov(45.0f, 640.0f, 640.0f, 0.01f, 100.0f) * matrix * view;
		//matrix = glm::perspectiveFov(45.0f, 640.0f , 480.0f, 0.01f, 100.0f) * matrix * view;
		//matrix *= glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f) * view; //  This works for 2D
		//matrix = glm::rotate(matrix, 3.14159f * 1.f, vec3(1.0f, 0.0f, 0.0f)); //  angle is in radians even though documentation says degrees
		matrix = GameServices::GetCameraManager()->GetActiveCamera()->GetViewProjectionMatrix();

		glUniformMatrix4fv(uWVP, 1, false, &matrix[0][0]);
	}

	// Draw the object
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
	//glPointSize(10.0f);
	//glDrawElements(GL_POINTS, m_IndexCount, GL_UNSIGNED_INT, 0);

	//if (glGetError() != 0)
	{
	//	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", glGetError());
	}


	// Disable attributes after using them
	glDisableVertexAttribArray(aPos);

	if (aUV != -1)
	{
		glDisableVertexAttribArray(aUV);
	}
    if (aNormal != -1)
	{
        glDisableVertexAttribArray(aNormal);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// Unbind this shader program
	glUseProgram(0);
	*/
}
