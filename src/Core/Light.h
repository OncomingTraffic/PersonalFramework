#ifndef LIGHT_H
#define LIGHT_H

#include "Core/GameObject.h"
#include "Core/Component.h"
#include <gl\GLU.h>
#include <glm\glm.hpp>


class Light : public Component
{

public:
	Light();
	Light(glm::vec4 color);
	virtual ~Light();

	virtual bool Initialize() = 0;

	glm::vec4 GetColor();

	void SetColor(glm::vec4 color);

protected:

	//TODO: Should this be a vec3 instead?
	glm::vec4 m_Color;
};


#endif LIGHT_H