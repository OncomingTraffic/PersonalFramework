#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Renderer.h"
#include <memory>
#include "../AABB.h"

class Mesh;
class Material;


class MeshRenderer : public Renderer
{
public:

	MeshRenderer();
	virtual ~MeshRenderer();

	virtual bool Initialize();

	virtual void Cleanup();

	virtual void Draw();

	// Get the Mesh that this Renderer is rendering
	std::shared_ptr<Mesh> GetMesh();

	// Set the Mesh for this Renderer to render
	void SetMesh(std::shared_ptr<Mesh> mesh);

	//TODO: Make this function return a const reference
	AABB GetBoundingBox();

	void SetBoundingBoxDimensions(glm::vec3 minBounds, glm::vec3 maxBounds);

	std::string GetType();

protected:

	// The mesh that is rendered by this component
	std::shared_ptr<Mesh> m_Mesh;

	// The Axis Aligned Bounding Box for this mesh
	//TODO: See if other types of renderers will need Bounding Volumes
	AABB m_BoundingBox;


};

#endif //MESHRENDERER_H