#include "Renderer.h"
#include "GameObject.h"
#include "../RenderingManager.h"


Renderer::Renderer() : m_ShaderProgram(nullptr), m_Culled(true)
{
	//
}

Renderer::~Renderer()
{
	m_ShaderProgram = nullptr;
}

std::shared_ptr<Material> Renderer::GetMaterial()
{
	return m_Material;
}

void Renderer::SetMaterial(std::shared_ptr<Material> aMaterial)
{
	m_Material = aMaterial;
}

glm::mat4x4& Renderer::GetModelMatrix()
{
	return m_Parent->GetModelMatrix();
}

RenderingManager* Renderer::GetRenderingManager()
{
	return m_RenderingManager;
}

void Renderer::SetCulled(bool aCull)
{
	m_Culled = aCull;
}

bool Renderer::IsCulled()
{
	return m_Culled;
}