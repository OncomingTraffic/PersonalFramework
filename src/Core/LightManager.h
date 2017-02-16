#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H


#include <vector>
#include <memory>
#include <GL\glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>


class Light;
class PointLight;


class LightManager
{
public:

	LightManager();
	~LightManager();

	glm::vec3 GetAmbientColor();
	void SetAmbientColor(glm::vec3 color);

	const std::vector<PointLight*>& GetPointLights();

	glm::vec3 GetDirectionalLightColor();
	glm::vec3 GetDirectionalLightNormal();
	void SetDirectionalLightColor(glm::vec3 color);
	void SetDirectionalLightNormal(glm::vec3 normal);


private:

	std::vector<PointLight*> m_PointLights;

	// TODO: Allow for there to be multiple directional lights

	// The color of the directional light
	glm::vec3 m_DirectionalLightColor;

	// The normalized direction of the directional light
	glm::vec3 m_DirectionalLightNormal;

	// The Ambient Light is the global light that affects all objects in the scene. There is only one Ambient Light
	glm::vec3 m_AmbientColor;
};

#endif LIGHTMANAGER_H