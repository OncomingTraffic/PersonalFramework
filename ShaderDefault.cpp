#include "ShaderDefault.h"
#include <SDL.h>
#include "src\Core\Scene.h"
#include "RenderingManager.h"
#include "BaseObject.h"
#include "src\Core\Camera.h"
#include "src\Core\PointLight.h"


ShaderDefault::ShaderDefault() : Shader(), m_aPos(-1), m_aNormal(-1), m_aUV(-1)
{
	m_RenderQueue = 100;
}

ShaderDefault::~ShaderDefault()
{
	//
}

bool ShaderDefault::Initialize()
{
	if (!LoadShader("Default.vert", "Default.frag"))
	//if (!LoadShader("Shaders/Vert.vert", "Shaders/Frag.frag"))
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
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Not all attributes were found in shader Default\n");
	}

	m_uCameraPos = glGetUniformLocation(m_Program, "u_CameraPos");
	m_uAmbientColor = glGetUniformLocation(m_Program, "u_AmbientColor");
	m_uDirectionalColor = glGetUniformLocation(m_Program, "u_DirectionalLight.Color");
	m_uDirectionalNormal = glGetUniformLocation(m_Program, "u_DirectionalLight.Direction");

	if (m_uCameraPos < 0 || m_uAmbientColor < 0 || m_uDirectionalColor < 0 || m_uDirectionalNormal < 0)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Not all uniforms were found in shader Default\n");
		//SDL_assert(false);
	}

	// Unbind this shader program
	glUseProgram(0);

	return true;
}

void ShaderDefault::SetupAttributes()
{
	//TODO: Use VAOs
	glEnableVertexAttribArray(m_aPos);
	glVertexAttribPointer(m_aPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, Position));

	glEnableVertexAttribArray(m_aNormal);
	glVertexAttribPointer(m_aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, Normal));

	glEnableVertexAttribArray(m_aUV);
	glVertexAttribPointer(m_aUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, UV));

	

}

void ShaderDefault::SetupSharedUniforms(RenderingManager* aRenderingManager)
{
	if (m_uCameraPos != -1)
	{
		//TODO: Change this to something less gross
		glm::vec3 pos = aRenderingManager->GetActiveCamera()->GetParent()->GetPosition();
		glUniform3f(m_uCameraPos, pos.x, pos.y, pos.z);
	}

	std::vector<PointLight*> pointLights = aRenderingManager->GetNearestLights(glm::vec3());
	std::string posNames[4] = { "u_Light[0].Pos", "u_Light[1].Pos", "u_Light[2].Pos", "u_Light[3].Pos" };
	std::string colorNames[4] = { "u_Light[0].Color", "u_Light[1].Color", "u_Light[2].Color", "u_Light[3].Color" };
	std::string attenNames[4] = { "u_Light[0].Attenuation", "u_Light[1].Attenuation", "u_Light[2].Attenuation", "u_Light[3].Attenuation" };

	SDL_assert(pointLights.size() == 4);

	//TEMPORARY
	for (int i = 0; i < 4; ++i)
	{
		if (pointLights[i] != nullptr)
		{
			GLint light = glGetUniformLocation(GetProgram(), posNames[i].c_str());
			if (light != -1)
			{
				glm::vec3 pos = pointLights[i]->GetParent()->GetPosition();
				glUniform3f(light, pos.x, pos.y, pos.z);
			}

			GLint color = glGetUniformLocation(GetProgram(), colorNames[i].c_str());
			if (color != -1)
			{
				glm::vec4 col = pointLights[i]->GetColor();
				glUniform3f(color, col.x, col.y, col.z);
			}

			GLint atten = glGetUniformLocation(GetProgram(), attenNames[i].c_str());
			if (atten != -1)
			{
				glm::vec3 attenuation = pointLights[i]->GetAttentuation();
				glUniform3f(atten, attenuation.x, attenuation.y, attenuation.z);
			}
		}
	}

	//TEMPORARY

		


	if (m_uAmbientColor != -1)
	{
		glm::vec3 ambientColor = aRenderingManager->GetAmbientColor();
		glUniform4f(m_uAmbientColor, ambientColor.x, ambientColor.y, ambientColor.z, 1);
	}

	if (m_uDirectionalNormal != -1)
	{
		glm::vec3 normal = aRenderingManager->GetDirectionalLightNormal();
		glUniform3f(m_uDirectionalNormal, normal.x, normal.y, normal.z);
	}

	if (m_uDirectionalColor != -1)
	{
		glm::vec3 color = aRenderingManager->GetDirectionalLightColor();
		glUniform3f(m_uDirectionalColor, color.x, color.y, color.z);
	}
}

void ShaderDefault::EnableGLCapabilities()
{
	glEnable(GL_BLEND);
	//glEnable(GL_CULL_FACE); //TODO: Enable culling after models are fixed. Faces are clockwise on models right now
	//glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}

void ShaderDefault::DisableGLCapabilities()
{
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}