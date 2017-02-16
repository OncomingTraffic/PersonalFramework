#include "RenderingManager.h"
#include <algorithm>
#include "src\Core\Renderer.h"
#include "src\Core\Shader.h"
#include "src\Core\Camera.h"
#include "Material.h"
#include <SDL.h>
#include "BaseObject.h"
#include "src\Core\GameServices.h"
#include "ShaderColor.h"

RenderingManager::RenderingManager(Scene* aParentScene) : m_ParentScene(aParentScene), m_ActiveCamera(nullptr), m_UseBruteForceRendering(false), m_MaxPointLights(4)
{
	m_AmbientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	m_DirectionalLightColor = glm::vec3(0.5f, 0.5f, 0.3f);
	m_DirectionalLightNormal = glm::vec3(-0.4f, -1, 0);

	//TODO: Move this to an Initialize method and create a RootNode class
	m_Quadtree = std::make_unique<QuadtreeNode>(0, 4, 4, glm::vec2(-100, -100), glm::vec2(100, 100), nullptr);

	m_DebugRenderer = std::make_unique<DebugRenderer>();
	GameServices::GetShaderManager()->AddShader("Color", std::make_shared<ShaderColor>());
	std::shared_ptr<Shader> shader = GameServices::GetShaderManager()->GetShader(std::string("Color"));
	m_DebugRenderer->Initialize(this, shader);
}

RenderingManager::~RenderingManager()
{
	m_Quadtree.reset();
}

void RenderingManager::Draw()
{
	if (m_ActiveCamera == nullptr)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "No active camera");
		return;
	}

	m_ActiveCamera->CalculateViewProjectionMatrix();

	if (!m_UseBruteForceRendering)
	{
		m_Quadtree->FrustumCull(m_ActiveCamera->GetViewFrustum());
	}

	int drawCount = 0;
	int totalCount = 0;

	// Iterate through all of the shaders
	for (auto shaderIter = m_RenderBatches.begin(); shaderIter != m_RenderBatches.end(); ++shaderIter)
	{
		// Set the active shader
		shaderIter->first->UseShader();
	
		// Enable OpenGL capabilities
		shaderIter->first->EnableGLCapabilities();

		// Set uniforms that are shared by this shader
		shaderIter->first->SetupSharedUniforms(this);
	
		// Iterate through all the renderers that use this shader
		for (auto renderer = shaderIter->second.begin(); renderer != shaderIter->second.end(); ++renderer)
		{
			//TEMPORARY = don't draw if it is culled - may move this into the renderer Draw method
			if (m_UseBruteForceRendering || !(*renderer)->IsCulled())
			{
				drawCount++;
				// Dereference, since renderer is the iterator
				(*renderer)->Draw();
			}

			totalCount++;
		}
	
		// Disable OpenGL capabilities, so that they aren't active when the next shader renders
		shaderIter->first->DisableGLCapabilities();
	}

	// Set the active program to invalid
	glUseProgram(0);

	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Objects Drawn: %i / %i", drawCount, totalCount);

	//AABB test(-1, -1, -1, 2, 2, 2);

	//unsigned int result = m_ActiveCamera->GetViewFrustum().CheckIntersection(test);

	//glm::vec3 pos = m_ActiveCamera->GetParent()->GetPosition();
	////
	////Plane planes[6] = { Plane(glm::vec4(1, 0, 0, -pos.x + 5)), Plane(glm::vec4(-1, 0, 0, pos.x + 5)), Plane(glm::vec4(0, -1, 0, pos.y + 5)),
	////	Plane(glm::vec4(0, 1, 0, -pos.y + 5)), Plane(glm::vec4(0, 0, 1, -pos.z + 5)), Plane(glm::vec4(0, 0, -1, pos.z + 5)) };
	////
	//unsigned int result = AABB_INTERSECT;
	//
	//float dotProduct = 0;
	//
	//Plane planes[6] = { m_ActiveCamera->GetViewFrustum().planes[0],
	//	m_ActiveCamera->GetViewFrustum().planes[1] ,
	//	m_ActiveCamera->GetViewFrustum().planes[2] ,
	//	m_ActiveCamera->GetViewFrustum().planes[3] ,
	//	m_ActiveCamera->GetViewFrustum().planes[4] ,
	//	m_ActiveCamera->GetViewFrustum().planes[5] };
	//
	//for (int i = 0; i < 6; ++i)
	//{
	//	planes[i].Normalize();
	//	dotProduct = glm::dot(planes[i].normal, glm::vec3(0, -9.9f, 0)) + planes[i].distance;
	//	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Dot: [%i] = %f", i, dotProduct);
	//	if (dotProduct < 0.0f)
	//	{
	//		result = AABB_OUTSIDE;
	//		break;
	//	}
	//}
	//
	////SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Position: %f, %f, %f", pos.x, pos.y, pos.z);
	//
	//
	//if (result == AABB_INSIDE) { SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Inside"); }
	//else if (result == AABB_INTERSECT) { SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Intersect"); }
	//else if (result == AABB_OUTSIDE) { SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Outside"); }

	//TODO:
	//m_Quadtree->DebugDraw(m_DebugRenderer.get());
}

void RenderingManager::AddShader(std::shared_ptr<Shader> shader)
{
	m_RenderBatches.emplace(shader, std::vector<Renderer*>());
}

void RenderingManager::AddRenderer(Renderer* aRenderer)
{
	//TODO: Fix this. Consider using a Set instead of a Vector for unique elements
	m_RenderBatches[aRenderer->GetMaterial()->GetShader()].push_back(aRenderer);

	m_Quadtree->AddRenderer((MeshRenderer*)aRenderer);
}

void RenderingManager::RemoveRenderer(Renderer* renderer)
{
	// TEMPORARY
	// TODO: Do something about this garbage
	// Shared Pointers will not work here. Rendering manager shouldn't have ownership of the renderers
	//std::vector<std::shared_ptr<Renderer>>::iterator iter = std::find(m_RenderBatches[renderer->GetMaterial()->GetShader()].begin(), m_RenderBatches[renderer->GetMaterial()->GetShader()].end(), std::shared_ptr<Renderer>(renderer));
	std::vector<Renderer*>* batch = &m_RenderBatches[renderer->GetMaterial()->GetShader()];
	std::vector<Renderer*>::iterator iter = std::find_if(batch->begin(), batch->end(), [&](Renderer* p){return p == renderer; });
	if (iter != batch->end())
	{
		batch->erase(iter);
	}
}


void RenderingManager::AddCamera(Camera* aCamera, bool aMakeActive)
{
	if (aCamera != nullptr)
	{
		m_Cameras.push_back(aCamera);

		//TODO: Find a better way to set the active camera
		if (aMakeActive || m_ActiveCamera == nullptr)
		{
			m_ActiveCamera = aCamera;
		}
	}


}

void RenderingManager::RemoveCamera(std::string aCameraTag)
{
	for (unsigned int i = 0; i < m_Cameras.size(); ++i)
	{
		if (m_Cameras.at(i)->GetTag() != aCameraTag)
		{
			if (m_Cameras.at(i) != nullptr)
			{
				delete m_Cameras.at(i);
				m_Cameras.at(i) = nullptr;
			}

			m_Cameras.erase(m_Cameras.begin() + i);
		}
	}
}

void RenderingManager::SetActiveCamera(std::string aCameraTag)
{
	for (unsigned int i = 0; i < m_Cameras.size(); ++i)
	{
		if (m_Cameras.at(i)->GetTag() != aCameraTag)
		{
			m_ActiveCamera = m_Cameras.at(i);
			break;
		}
	}
}

Camera* RenderingManager::GetActiveCamera()
{
	SDL_assert(m_ActiveCamera != nullptr);

	return m_ActiveCamera;
}


void RenderingManager::SetBruteForceRendering(bool aUseBruteForce)
{
	m_UseBruteForceRendering = aUseBruteForce;
}


glm::vec3 RenderingManager::GetAmbientColor()
{
	return m_AmbientColor;
}

void RenderingManager::SetAmbientColor(glm::vec3 aColor)
{
	m_AmbientColor = aColor;
}

glm::vec3 RenderingManager::GetDirectionalLightColor()
{
	return m_DirectionalLightColor;
}

void RenderingManager::SetDirectionalLightColor(glm::vec3 aColor)
{
	m_DirectionalLightColor = aColor;
}

glm::vec3 RenderingManager::GetDirectionalLightNormal()
{
	return m_DirectionalLightNormal;
}

void RenderingManager::SetDirectionalLightNormal(glm::vec3 aNormal)
{
	m_DirectionalLightNormal = aNormal;
}

void RenderingManager::AddPointLight(PointLight* aLight)
{
	m_PointLights.push_back(aLight);
}

void RenderingManager::RemovePointLight(PointLight* aLight)
{
	std::vector<PointLight*>::iterator iter = std::find(m_PointLights.begin(), m_PointLights.end(), aLight);
	if (iter != m_PointLights.end())
	{
		m_PointLights.erase(iter);
	}
}

std::vector<PointLight*> RenderingManager::GetNearestLights(glm::vec3 aPosition)
{
	//TEMPORARY - this seems like an inefficient way to do this
	std::vector<PointLight*> lights = { nullptr, nullptr, nullptr, nullptr };

	int index = 0;

	for (unsigned int i = 0; i < m_PointLights.size(); ++i)
	{
		if (index < 4)
		{
			lights[index] = m_PointLights.at(i);
			++index;
		}
		//TODO: Else 
	}

	return lights;
}