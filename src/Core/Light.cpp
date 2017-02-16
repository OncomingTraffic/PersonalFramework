#include "Light.h"

Light::Light() : Component(), m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{
	//m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // White Light
	//m_Attenuation = glm::vec3(1.0f, 0.1f, 0.01f);
}

Light::Light(glm::vec4 aColor) : m_Color(aColor)
{
	//
}

Light::~Light()
{
	//
}

glm::vec4 Light::GetColor()
{
	return m_Color;
}

void Light::SetColor(glm::vec4 aColor)
{
	m_Color = aColor;
}