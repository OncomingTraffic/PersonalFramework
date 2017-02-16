#include "PointLight.h"
#include "Scene.h"
#include "../RenderingManager.h"

PointLight::PointLight() : Light(), m_Attenuation(1.0f, 0.1f, 0.01f)
{
	//
}

PointLight::PointLight(glm::vec4 aColor, glm::vec3 aAttenuation) : Light(aColor), m_Attenuation(aAttenuation)
{
	//
}

PointLight::~PointLight()
{
	//
}


bool PointLight::Initialize()
{
	// Add to light manager
	m_RenderingManager = m_Parent->GetParentScene()->GetRenderingManager().get();

	m_RenderingManager->AddPointLight(this);

	return true;
}

void PointLight::SetAttenuation(glm::vec3 aAttenuation)
{
	m_Attenuation = aAttenuation;
}

glm::vec3 PointLight::GetAttentuation()
{
	return m_Attenuation;
}

std::string PointLight::GetType()
{
	return std::string("PointLight");
}