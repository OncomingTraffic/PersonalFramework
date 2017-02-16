#ifndef RENDERER_H
#define RENDERER_H

#include "Component.h"
#include <glm\glm.hpp>

class ShaderProgram;
class Material;
class RenderingManager;


class Renderer : public Component
{
public:

	Renderer();
	virtual ~Renderer();

	//virtual bool Initialize(); // This method is pure virtual in the Component class

	virtual void Draw() = 0;

	std::shared_ptr<Material> GetMaterial();
	void SetMaterial(std::shared_ptr<Material> material);

	glm::mat4x4 &GetModelMatrix();

	RenderingManager* GetRenderingManager();

	// Set if the renderer is being culled. A culled renderer is not drawn
	void SetCulled(bool cull);

	// Is the renderer being culled this frame
	bool IsCulled();

protected:
	
	ShaderProgram* m_ShaderProgram;

	// The material determines which shader is used to render this object,
	// and sets uniforms in that shader. All renderers have a material
	std::shared_ptr<Material> m_Material;

	// The RenderingManager that this renderer belongs to and is rendered by
	RenderingManager* m_RenderingManager;

	bool m_Culled;
};


#endif //RENDERER_H