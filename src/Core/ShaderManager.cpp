#include "ShaderManager.h"
#include "Shader.h"
#include <SDL.h>

ShaderManager::ShaderManager()
{
	//
}

ShaderManager::~ShaderManager()
{
	CloseAllShaders();
}

std::shared_ptr<Shader> ShaderManager::GetShader(std::string &aShaderName)
{
	// Check if the shader program is already loaded
	std::map<std::string, std::shared_ptr<Shader>>::iterator index = m_ShaderPrograms.find(aShaderName);

	// If the shader was not found
	if (index == m_ShaderPrograms.end())
	{
		return nullptr;
	}
	else
	{
		// Return the map value at the found index
		return index->second;
	}
}

void ShaderManager::AddShader(std::string aShaderName, std::shared_ptr<Shader> aShader)
{
	// Check if the shader program is already loaded
	std::map<std::string, std::shared_ptr<Shader>>::iterator index = m_ShaderPrograms.find(aShaderName);

	// If the shader was not found
	if (index != m_ShaderPrograms.end())
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Shader was already added: %s", aShaderName);
		return;
	}
	if (aShader == nullptr)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Shader is null");
		return;
	}

	aShader->Initialize();

	m_ShaderPrograms[aShaderName] = aShader;
}

void ShaderManager::CloseShader(std::string aShaderName)
{
	std::map<std::string, std::shared_ptr<Shader>>::iterator index = m_ShaderPrograms.find(aShaderName);
	if (index != m_ShaderPrograms.end())
	{
		if (index->second != nullptr)
		{
			if (!index->second.unique())
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Shader being closed is still referenced: %s", aShaderName);
			}

			index->second.reset();
			//index->second = nullptr;
		}
		m_ShaderPrograms.erase(index);
	}
}

void ShaderManager::CloseAllShaders()
{
	// Go through and close each font in the map
	for (std::map<std::string, std::shared_ptr<Shader>>::iterator index = m_ShaderPrograms.begin(); index != m_ShaderPrograms.end();)
	{
		CloseShader(index->first); // This looks stupid. TODO: Fix it
		index = m_ShaderPrograms.begin();
	}
}