#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"
#include <gl\GLU.h>
#include <glm\glm.hpp>


class RenderingManager;

class PointLight : public Light
{
public:

	PointLight();
	PointLight(glm::vec4 color, glm::vec3 attenuation = glm::vec3(1.0f, 0.1f, 0.01f));
	virtual ~PointLight();

	virtual bool Initialize();

	void SetAttenuation(glm::vec3 attenuation);

	glm::vec3 GetAttentuation();


	//TODO: Cleanup

	std::string GetType();

protected:

	// Lights have a vec3 as Attenuation to allow for different Attenuation models
	glm::vec3 m_Attenuation;

	RenderingManager* m_RenderingManager;

};


#endif POINTLIGHT_H