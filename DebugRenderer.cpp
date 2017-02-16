#include "DebugRenderer.h"
#include "RenderingManager.h"
#include <SDL.h>
#include "src\Core\Camera.h"


DebugRenderer::DebugRenderer()
{
	//
}

DebugRenderer::~DebugRenderer()
{
	//
}


bool DebugRenderer::Initialize(RenderingManager* aManager, std::shared_ptr<Shader> aShader)
{
	m_RenderingManager = aManager;

	m_Shader = aShader;

	if (m_VBO == 0)
	{
		glGenBuffers(1, &m_VBO);
	}
	// Generate a new index buffer if there isn't one already
	if (m_IBO == 0)
	{
		glGenBuffers(1, &m_IBO);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Buffer the vertex data

	VertexFormat vertices[4] = { VertexFormat(-0.5f, 0.0f, -0.5f, 0.f, 0.f, 0.f, 1.f, 0.f),
		VertexFormat(0.5f, 0.0f, -0.5f, 1.f, 0.f, 0.f, 1.f, 0.f),
		VertexFormat(-0.5f, 0.0f, 0.5f, 0.f, 1.f, 0.f, 1.f, 0.f),
		VertexFormat(0.5f, 0.0f, 0.5f, 1.f, 1.f, 0.f, 1.f, 0.f) };

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * 4, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint indices[6] = {0, 1, 2, 1, 3, 2};

		// Bind the VBO to buffer the data into
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	// Buffer the vertex data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

void DebugRenderer::DrawPlane(glm::vec3 aPosition, glm::quat aRotation, glm::vec3 aScale, glm::vec4 aColor)
{
	GLuint shaderProgram = m_Shader->GetProgram();

	glUseProgram(shaderProgram);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	// Bind the vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	GLint aPos = glGetAttribLocation(shaderProgram, "a_Position");
	GLint aUV = glGetAttribLocation(shaderProgram, "a_UV");
	GLint aNormal = glGetAttribLocation(shaderProgram, "a_Normal");

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


	Camera* camera = m_RenderingManager->GetActiveCamera();

	//glUniform3fv(m_uLight, 1, &camera->GetPosition()[0]);
	glm::mat4 matrix = glm::translate(aPosition) * mat4_cast(aRotation) * glm::scale(aScale);
	matrix = camera->GetViewProjectionMatrix() * matrix;

	GLint uWVP = glGetUniformLocation(shaderProgram, "u_WVP");
	if (uWVP != -1)
	{
		glUniformMatrix4fv(uWVP, 1, false, &matrix[0][0]);
	}

	GLint uColor = glGetUniformLocation(shaderProgram, "u_Color");
	if (uColor != -1)
	{
		glUniform4f(uColor, aColor.r, aColor.g, aColor.b, aColor.a);
	}


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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

	glUseProgram(0);
}