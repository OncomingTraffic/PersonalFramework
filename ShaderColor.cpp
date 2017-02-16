#include "ShaderColor.h"
#include <SDL.h>
#include "src\Core\Scene.h"
#include "RenderingManager.h"
#include "BaseObject.h"


ShaderColor::ShaderColor() : Shader(), m_aPos(-1), m_aNormal(-1), m_aUV(-1)
{
	m_RenderQueue = 100;
}

ShaderColor::~ShaderColor()
{
	//
}

bool ShaderColor::Initialize()
{
	if (!LoadShader("Shaders/Color.vert", "Shaders/Color.frag"))
	{
		return false;
	}

	// Set the active shader
	glUseProgram(m_Program);

	// Get attribute locations in the shader
	m_aPos = glGetAttribLocation(m_Program, "a_Position");
	m_aNormal = glGetAttribLocation(m_Program, "a_Normal");
	m_aUV = glGetAttribLocation(m_Program, "a_UV");

	// Check if all attributes were found within the shader
	if (m_aPos < 0 || m_aNormal < 0 || m_aUV < 0)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Not all attributes were found in shader Color\n");
	}

	// Unbind this shader program
	glUseProgram(0);

	return true;
}

void ShaderColor::SetupAttributes()
{
	//TODO: Use VAOs
	glEnableVertexAttribArray(m_aPos);
	glVertexAttribPointer(m_aPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, Position));

	if (m_aNormal != -1)
	{
		glEnableVertexAttribArray(m_aNormal);
		glVertexAttribPointer(m_aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, Normal));
	}

	if (m_aUV != -1)
	{
		glEnableVertexAttribArray(m_aUV);
		glVertexAttribPointer(m_aUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, UV));
	}
}

void ShaderColor::EnableGLCapabilities()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}

void ShaderColor::DisableGLCapabilities()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}