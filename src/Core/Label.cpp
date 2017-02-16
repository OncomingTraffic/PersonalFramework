#include "Label.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Texture.h"
#include "GameServices.h"

Label::Label(std::string &aText, std::string &aFontPath, unsigned int aFontSize, SDL_Color aFontColor, glm::vec3 aPosition, glm::vec3 aRotation, glm::vec3 aScale, bool aIsEnabled) : GameObject(aPosition, aRotation, aScale, aIsEnabled),
	Mesh(), m_Text(aText), m_Font(nullptr), m_FontSize(aFontSize), m_FontColor(aFontColor), m_Texture(nullptr), m_Width(0.0f), m_Height(0.0f)
{
	// TODO: Don't do anyhting other than initializing in the constructor
	//		 Any code that can possibly fail should be done in a seperately function
	//		 NO data should ever be created in a constructor (not including things being initialized)
	LoadFont(aFontPath);
	LoadTexture();
}

Label::~Label()
{
	// The Texture class will delete the texture it loaded with OpenGL automatically when destroyed
	if (m_Texture != nullptr)
	{
		delete m_Texture;
		m_Texture = nullptr;
	}
}

bool Label::LoadFont(std::string &aFontPath)
{
	m_Font = GameServices::GetFontManager()->AddFont(aFontPath, 42);

	return true;
}

bool Label::LoadTexture()
{
	if (m_Texture == nullptr)
	{
		m_Texture = new Texture();
	}
	SDL_Surface* textSurface = TTF_RenderText_Blended(m_Font, m_Text.c_str(), m_FontColor);
	//SDL_SetColorKey(textSurface, 1, SDL_MapRGB(textSurface->format, 255, 255, 255)); // Unneeded
	
	if (m_Texture->LoadTexture(textSurface))
	{
		// If the width and height haven't been set yet, set them to the loaded texture's original dimensions
		if (m_Width == 0.0f) { m_Width = (float)m_Texture->GetWidth(); }
		if (m_Height == 0.0f) { m_Height = (float)m_Texture->GetHeight(); }

		SetupArrayBuffers();

		SDL_FreeSurface(textSurface);

		return true;
	}

	if (textSurface)
	{
		SDL_FreeSurface(textSurface);
	}

	return false;
}

void Label::SetupArrayBuffers()
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
		//--			Position	--		UV		--		Color	--
		vertices[0] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0, 1 }; // Bottom left
		vertices[1] = { m_Width, 0.0f, 0.0f, 1.0f, 1.0f, 0, 0, 1 }; // Bottom right
		vertices[2] = { 0.0f, m_Height, 0.0f, 0.0f, 0.0f, 0, 0, 1 }; // Top left
		vertices[3] = { m_Width, m_Height, 0.0f, 1.0f, 0.0f, 0, 0, 1 }; // Top right
		vertices[0].Position = { -m_Width / 2, -m_Height / 2, 0.0f };
		vertices[1].Position = { m_Width / 2, -m_Height / 2, 0.0f };
		vertices[2].Position = { -m_Width / 2, m_Height / 2, 0.0f };
		vertices[3].Position = { m_Width / 2, m_Height / 2, 0.0f };


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


void Label::Update(double delta)
{
	//
}

void Label::Draw()
{
	/*
	// Store the shader program handle
	GLuint shaderProgram = 0;// m_Shader->GetProgram();

	// Enable GL capabilities for this object to use to render
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the active shader to the one used by this object
	glUseProgram(shaderProgram);

	//TODO: research Vertex Array Objects (VAO) and see if they are more suitable
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
        glVertexAttribPointer(aNormal, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, Normal));
	}

	// Set up uniforms
	if (uTex != -1)
	{
		// By default, textures are applied to the texture unit GL_TEXTURE0. This only needs to change if you are appliying multiple textures to the same mesh
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

		mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		mat4 matrix = {
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		
		//matrix *= glm::perspective(45.0f, 640.0f / 640.0f, 0.01f, 100.0f) * view;
		//matrix *= glm::perspectiveFov(45.0f, 640.0f , 480.0f, 0.01f, 100.0f) * view;
		matrix = glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f) * view; //  This works for 2D

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

	// Unbind buffers that this object uses
	// TODO: See if this is needed. These buffers should be overridden whenever anything draws and binds new ones anyway
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Disable the GL capabilities that this object uses
	glDisable(GL_BLEND);

	// Unbind this shader program
	// TODO: This may also be unnecessary, especially if I can batch a bunch of the same draw calls together
	glUseProgram(0);
	*/
}


void Label::SetText(std::string aText)
{
	m_Text = aText;
	m_Width = 0.0f;
	m_Height = 0.0f; // TODO: Find a beter way to reset the width and height - see if it is necessary for the user to ever set these manually
	LoadTexture();
}