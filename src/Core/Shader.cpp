#include "Shader.h"
#include <SDL.h>
#include "FileReader.h"
#include "../RenderingManager.h"


Shader::Shader() : m_VertShaderString(nullptr), m_FragShaderString(nullptr), m_VertShader(0), m_FragShader(0), m_Program(0), m_RenderQueue(0)
{
	//
}

Shader::~Shader()
{
	if (m_VertShaderString)
	{
		delete[] m_VertShaderString;
	}
	if (m_FragShaderString)
	{
		delete[] m_FragShaderString;
	}
	 
	glDetachShader(m_Program, m_VertShader);
	glDetachShader(m_Program, m_FragShader);

	if (m_VertShader)
	{
		glDeleteShader(m_VertShader);
	}
	if (m_FragShader)
	{
		glDeleteShader(m_FragShader);
	}
	if (m_Program)
	{
		glDeleteProgram(m_Program);
	}

	m_VertShader = 0;
	m_FragShader = 0;
	m_Program = 0;

}


bool Shader::LoadShader(const char* aVertShaderFile, const char* aFragShaderFile)
{
	bool success = true;

	// Create a new GL program
	m_Program = glCreateProgram();
	if (m_Program == 0)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(ShaderProgram) OpenGL program could not be created. glError: %i", glGetError());
		return false;
	}
	else
	{
		// Create the vertex shader
		m_VertShader = glCreateShader(GL_VERTEX_SHADER);
		if (m_VertShader == 0)
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(ShaderProgram) Vertex shader could not be created.");
			return false;
		}
		else
		{
			// Load the vertex shader string from a file
			m_VertShaderString = LoadFile(aVertShaderFile);
			if (m_VertShaderString == nullptr)
			{
				glDeleteShader(m_VertShader);

				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(ShaderProgram) Vertex shader file could not be read.");
				return false;
			}

			// Set the vertex shader source
			glShaderSource(m_VertShader, 1, &m_VertShaderString, 0);

			// Compile the vertex shader
			glCompileShader(m_VertShader);

			GLint compileSuccess = GL_FALSE;

			// Make sure the shader compiled correctly
			glGetShaderiv(m_VertShader, GL_COMPILE_STATUS, &compileSuccess);
			if (compileSuccess != GL_TRUE)
			{
				glDeleteShader(m_VertShader);

				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(ShaderProgram) Vertex shader file could not compile: %s\n", aVertShaderFile);
				return false;
			}

			// Attach the compiled shader to the shader program
			glAttachShader(m_Program, m_VertShader);
		}

		// Create the fragment shader
		m_FragShader = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_FragShader == 0)
		{
			glDetachShader(m_Program, m_VertShader);
			glDeleteShader(m_VertShader);

			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(ShaderProgram) Fragment shader could not be created.");
			return false;
		}
		else
		{
			// Load the fragment shader string from a file
			m_FragShaderString = LoadFile(aFragShaderFile);
			if (m_FragShaderString == nullptr)
			{
				glDetachShader(m_Program, m_VertShader);
				glDeleteShader(m_VertShader);
				glDeleteShader(m_FragShader);

				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(ShaderProgram) Fragment shader file could not be read.");
				return false;
			}

			// Set the fragment shader source
			glShaderSource(m_FragShader, 1, &m_FragShaderString, 0);

			// Compile the fragment shader
			glCompileShader(m_FragShader);

			GLint compileSuccess = GL_FALSE;

			// Make sure the shader compiled correctly
			glGetShaderiv(m_FragShader, GL_COMPILE_STATUS, &compileSuccess);
			if (compileSuccess != GL_TRUE)
			{
				glDetachShader(m_Program, m_VertShader);
				glDeleteShader(m_VertShader);
				glDeleteShader(m_FragShader);

				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(ShaderProgram) Fragment shader file could not compile: %s", aFragShaderFile);
				return false;
			}

			// Attach the compiled shader to the shader program
			glAttachShader(m_Program, m_FragShader);
		}

		// Link the shader program
		glLinkProgram(m_Program);

		GLint linkSuccess = 0;

		// Detach and delete shaders objects after program links
		// Frees up memory and avoids memory leaks
		// TODO: Don't delete shaders if other programs will use them
		glDetachShader(m_Program, m_VertShader);
		glDeleteShader(m_VertShader);

		glDetachShader(m_Program, m_FragShader);
		glDeleteShader(m_FragShader);


		// Make sure the program was linked correctly
		glGetProgramiv(m_Program, GL_LINK_STATUS, &linkSuccess);
		if (linkSuccess != GL_TRUE)
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "(ShaderProgram) Program could not link.");
			return false;
		}
	}

	return true;
}

GLuint Shader::GetProgram()
{
	return m_Program;
}

unsigned int Shader::GetRenderQueue()
{
	return m_RenderQueue;
}

void Shader::UseShader()
{
	glUseProgram(m_Program);
}

void Shader::SetupSharedUniforms(RenderingManager* aRenderingManager)
{
	//
}

void Shader::SetupAttributes()
{
	//
}

void Shader::EnableGLCapabilities()
{
	//
}

void Shader::DisableGLCapabilities()
{
	//
}