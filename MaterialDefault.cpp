#include "MaterialDefault.h"
#include "src\Core\Shader.h"
#include "src\Core\Renderer.h"
#include "src\Core\Texture.h"
#include "..\src\Core\GameServices.h"
#include "RenderingManager.h"



MaterialDefault::MaterialDefault() : Material(), m_aPos(-1), m_aUV(-1), m_aNormal(-1), m_uWVP(-1), m_uTex(-1)
{
	//
}

MaterialDefault::~MaterialDefault()
{
	//
}

bool MaterialDefault::Initialize()
{
	GLint shaderProgram = 0;

	m_Shader = GameServices::GetShaderManager()->GetShader(std::string("Default"));

	m_Texture = GameServices::GetResourceManager()->LoadResource<Texture>("Textures/DefaultNoAlpha.bmp");

	shaderProgram = m_Shader->GetProgram();

	//m_aPos = glGetAttribLocation(shaderProgram, "a_Position");
	//m_aUV = glGetAttribLocation(shaderProgram, "a_UV");
	//m_aNormal = glGetAttribLocation(shaderProgram, "a_Normal");
	m_uTex = glGetUniformLocation(shaderProgram, "u_Texture");
	m_uWVP = glGetUniformLocation(shaderProgram, "u_WVP");

	//"u_Light[0]"

	return true;
}

void MaterialDefault::SetupAttributes()
{
	m_Shader->SetupAttributes();
}

void MaterialDefault::SetupUniforms(Renderer* aRenderer)
{
	glm::mat4 matrix = aRenderer->GetModelMatrix();// * rotationMatrix * scaleMatrix; // Scale, then Rotate, then Translate

	GLint worldMatrix = glGetUniformLocation(m_Shader->GetProgram(), "u_ModelMatrix");
	if (worldMatrix != -1)
	{
		glUniformMatrix4fv(worldMatrix, 1, false, &matrix[0][0]);
	}

	if (m_uTex != -1 && m_Texture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0 + 0);

		// Bind the current texture to be used by the shader
		glBindTexture(GL_TEXTURE_2D, m_Texture->GetTextureID());

		// Set u_Texture in the shader to use texture unit 0 (GL_TEXTURE0)
		glUniform1i(m_uTex, 0);
	}

	Camera* camera = aRenderer->GetRenderingManager()->GetActiveCamera();

	//glUniform3fv(m_uLight, 1, &camera->GetPosition()[0]);

	matrix = camera->GetViewProjectionMatrix() * matrix;

	glUniformMatrix4fv(m_uWVP, 1, false, &matrix[0][0]);
}