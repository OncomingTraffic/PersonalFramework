#include "MeshRenderer.h"
#include "GameObject.h"
#include "Mesh.h"
#include "../Material.h"
#include "Shader.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "GameServices.h"
#include "../RenderingManager.h"

MeshRenderer::MeshRenderer() : Renderer(), m_Mesh(nullptr), m_BoundingBox()
{
	//
}

MeshRenderer::~MeshRenderer()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Renderer being deleted");
	
	Cleanup();
}

bool MeshRenderer::Initialize()
{
	m_RenderingManager = m_Parent->GetParentScene()->GetRenderingManager().get();

	if (m_RenderingManager != nullptr)
	{
		m_RenderingManager->AddRenderer(this);

		//TEMPORARY
		m_BoundingBox = AABB(-0.5f, -0.5f, -0.5f, 1, 1, 1);

		return true;
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "RenderingManager is null");
	
	return false;
}

void MeshRenderer::Draw()
{
	// Bind shader program
	// Enable GL capabilities
	// -Done by RenderingManager

	// Bind VBO and IBO
	m_Mesh->BindArrayBuffers();

	// Set attributes
	// TODO: Fix this
	m_Material->GetShader()->SetupAttributes();

	// Uniforms
	m_Material->SetupUniforms(this);

	glDrawElements(GL_TRIANGLES, m_Mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);

	m_Culled = true;
}


// Get the Mesh that this Renderer is rendering
std::shared_ptr<Mesh> MeshRenderer::GetMesh()
{
	return m_Mesh;
}

// Set the Mesh for this Renderer to render
void MeshRenderer::SetMesh(std::shared_ptr<Mesh> aMesh)
{
	m_Mesh = aMesh;
}


void MeshRenderer::Cleanup()
{
	if (m_RenderingManager != nullptr)
	{
		m_RenderingManager->RemoveRenderer(this);
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "RenderingManager is null");
	}
}

AABB MeshRenderer::GetBoundingBox()
{
	glm::vec3 pos = m_Parent->GetPosition();
	//TODO: Update bounding box when object moves (when model matrix is recalculated)
	AABB boundingBox = AABB(pos + m_BoundingBox.MinBounds, pos + m_BoundingBox.MaxBounds);

	return boundingBox;
}

void MeshRenderer::SetBoundingBoxDimensions(glm::vec3 aMinBounds, glm::vec3 aMaxBounds)
{
	m_BoundingBox.MinBounds = aMinBounds;
	m_BoundingBox.MaxBounds = aMaxBounds;
}

std::string MeshRenderer::GetType()
{
	return std::string("MeshRenderer");
}
