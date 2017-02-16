#include "Player.h"

#include <SDL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "Core/GameServices.h"
#include "Core/ObjLoader.h"
#include "../RigidBody.h"

Player::Player() : GameObject(), Mesh(), m_Velocity(glm::vec3(0, 0, 0)), m_IsIdle(true)
{
	SetupArrayBuffers();
}

Player::~Player()
{
	//
}


void Player::SetupArrayBuffers()
{

	//m_IsMeshLoaded = true;
}

void Player::Update(double aDelta)
{
	const Uint8* KeyStates = SDL_GetKeyboardState(NULL);

	if (m_IsIdle)
	{
		float speed = 500.0f;

		float angle = 60.0f * (float)M_PI / 180.0f;

		if (KeyStates[SDL_SCANCODE_UP])
		{
			m_Velocity.z = speed * cos(angle);
			m_Velocity.y = speed * sin(angle);
			m_IsIdle = false;
		}

		else if (KeyStates[SDL_SCANCODE_DOWN])
		{
			m_Velocity.z = -speed * cos(angle);
			m_Velocity.y = speed * sin(angle);
			m_IsIdle = false;
		}

		else if (KeyStates[SDL_SCANCODE_LEFT])
		{
			m_Velocity.x = -speed * cos(angle);
			m_Velocity.y = speed * sin(angle);
			m_IsIdle = false;
		}

		else if (KeyStates[SDL_SCANCODE_RIGHT])
		{
			m_Velocity.x = speed * cos(angle);
			m_Velocity.y = speed * sin(angle);
			m_IsIdle = false;
		}
	}

	// Don't update if the player is isn't moving
	else if (!m_IsIdle)
	{
		// Move based on frames instead of actual time
		float time = 1.0f / 60.0f;
		m_Position += m_Velocity * float(time);
		
		//m_Velocity -= m_Velocity * 1.5f * float(aDelta);
		m_Velocity.y -= 500.f * time;

		if (m_Velocity.y < 0)
		{
			// Collision with the ground
			if (m_Position.y <= 0)
			{
				m_Position.y = 0;
				m_Velocity.y *= -0.7f;
				if (m_Velocity.y < 70.0f)
				{
					m_Velocity.y = 0;
				}
				glm::vec3 vel = glm::normalize(glm::vec3(m_Velocity.x, 0, m_Velocity.z));
				m_Velocity.x -= abs(vel.x) * m_Velocity.x * 0.25f;
				m_Velocity.z -= abs(vel.z) * m_Velocity.z * 0.25f;
			}
		}

		if (glm::length(m_Velocity) < 1.5f)
		{
			m_IsIdle = true;
			m_Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}
}

/*
void Player::Draw()
{
	// Store the shader program handle
	//GLuint shaderProgram = m_Shader->GetProgram();
	GLuint shaderProgram = 0;

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
		mat4 scaleMat = glm::scale(m_Scale);
		mat4 rotationMat = mat4_cast(m_Rotation);
		// TODO: Store this matrix
		mat4 matrix = glm::translate(m_Position) * rotationMat * scaleMat;// * rotationMatrix * scaleMatrix; // Scale, then Rotate, then Translate
		
		matrix = GameServices::GetCameraManager()->GetActiveCamera()->GetViewProjectionMatrix() * matrix; // 

		glUniformMatrix4fv(uWVP, 1, false, &matrix[0][0]);
	}

	// Draw the object
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
	//glPointSize(10.0f);
	//glDrawElements(GL_POINTS, m_IndexCount, GL_UNSIGNED_INT, 0);

	//if (glGetError() != 0)
	{
		//	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "%s\n", glGetError());
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
}
*/

void Player::SetVelocity(glm::vec3 aVelocity)
{
	m_Velocity = aVelocity;
}

glm::vec3 Player::GetVelocity()
{
	return m_Velocity;
}

bool Player::IsIdle()
{
	return m_IsIdle;
}

void Player::UpdateRBPos(RigidBody* rb)
{
	btVector3 newPos = rb->GetRigidBody()->getWorldTransform().getOrigin();

	SetPosition(glm::vec3(newPos.getX(), newPos.getY(), newPos.getZ()));
	
}