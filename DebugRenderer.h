#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

#include <GL\glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <memory>
#include "src\Core\Shader.h"


class RenderingManager;


class DebugRenderer
{
public:

	DebugRenderer();
	~DebugRenderer();

	bool Initialize(RenderingManager* manager, std::shared_ptr<Shader> shader);

	void DrawPlane(glm::vec3 position, glm::quat rotation = glm::quat(), glm::vec3 scale = glm::vec3(1, 1, 1), glm::vec4 color = glm::vec4(1, 1, 1, 0.5f));


private:

	RenderingManager* m_RenderingManager;

	std::shared_ptr<Shader> m_Shader;

	//TODO: Move these to a "plane" object, so that other shapes can be drawn
	GLuint m_VBO;
	GLuint m_IBO;
};


#endif //DEBUGRENDERER_H