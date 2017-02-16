#include "LightManager.h"


LightManager::LightManager() : m_AmbientColor(0, 0, 0)
{
	//
}

LightManager::~LightManager()
{
	// Point Lights are components, so their deletion is handled by their parent BaseObject
}


glm::vec3 LightManager::GetAmbientColor()
{
	return m_AmbientColor;
}

void LightManager::SetAmbientColor(glm::vec3 aColor)
{
	m_AmbientColor = aColor;
}


glm::vec3 LightManager::GetDirectionalLightColor()
{
	return m_DirectionalLightColor;
}

glm::vec3 LightManager::GetDirectionalLightNormal()
{
	return m_DirectionalLightNormal;
}

void LightManager::SetDirectionalLightColor(glm::vec3 aColor)
{
	m_DirectionalLightColor = aColor;
}

void LightManager::SetDirectionalLightNormal(glm::vec3 aNormal)
{
	m_DirectionalLightNormal = aNormal;
}


const std::vector<PointLight*>& LightManager::GetPointLights()
{
	return m_PointLights;
}