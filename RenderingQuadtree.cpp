#include "RenderingQuadtree.h"
#include "src\Core\MeshRenderer.h"
#include <limits>
#include <SDL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "RenderingManager.h"
#include "src\Core\Camera.h"
#include "DebugRenderer.h"


QuadtreeNode::QuadtreeNode(unsigned short aDepth, unsigned short aMaxDepth, unsigned short aMaxRenderers, glm::vec2 aAABBMin, glm::vec2 aAABBMax, QuadtreeNode* aParent)
	: m_Depth(aDepth), m_MaxDepth(aMaxDepth), m_MaxRenderersPerNode(aMaxRenderers), m_BoundingBox(aAABBMin, aAABBMax), m_ParentNode(aParent), m_IsLeafNode(true)
{
	Culled = false;
}

QuadtreeNode::~QuadtreeNode()
{
	//
}


void QuadtreeNode::Split()
{
	glm::vec2 half = m_BoundingBox.MaxBounds - m_BoundingBox.MinBounds;
	half.x *= 0.5f;
	half.y *= 0.5f;

	m_TopLeft = std::make_unique<QuadtreeNode>(m_Depth + 1, m_MaxDepth, m_MaxRenderersPerNode, m_BoundingBox.MinBounds + glm::vec2(0, half.y), m_BoundingBox.MaxBounds - glm::vec2(half.x, 0), this);
	m_TopRight = std::make_unique<QuadtreeNode>(m_Depth + 1, m_MaxDepth, m_MaxRenderersPerNode, m_BoundingBox.MinBounds + half, m_BoundingBox.MaxBounds, this);
	m_BottomLeft = std::make_unique<QuadtreeNode>(m_Depth + 1, m_MaxDepth, m_MaxRenderersPerNode, m_BoundingBox.MinBounds, m_BoundingBox.MinBounds + half, this);
	m_BottomRight = std::make_unique<QuadtreeNode>(m_Depth + 1, m_MaxDepth, m_MaxRenderersPerNode, m_BoundingBox.MinBounds + glm::vec2(half.x, 0), m_BoundingBox.MaxBounds - glm::vec2(0, half.y), this);

	for (unsigned int i = 0; i < m_Renderers.size(); ++i)
	{
		AddRendererToChildNodes(m_Renderers.at(i));
	}

	m_Renderers.clear();

	m_IsLeafNode = false;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Node splitting");
}

unsigned short QuadtreeNode::AddRenderer(MeshRenderer* aRenderer)
{
	unsigned short intersectType = m_BoundingBox.Intersects(aRenderer->GetBoundingBox());

	// Don't add the renderer if it is outside this node's bounding box
	if (intersectType == AABBIntersectType::AABB_OUTSIDE)
	{
		return intersectType;
	}
	
	// Only add to this node if it's a leaf node
	if (m_IsLeafNode)
	{
		m_Renderers.push_back(aRenderer);
		if (m_Renderers.size() >= m_MaxRenderersPerNode && m_Depth < m_MaxDepth)
		{
			Split();
		}
		return intersectType;
	}
	else
	{
		AddRendererToChildNodes(aRenderer);
		return intersectType;
	}
}

void QuadtreeNode::AddRendererToChildNodes(MeshRenderer* aRenderer)
{
	if (m_TopLeft->AddRenderer(aRenderer) == AABBIntersectType::AABB_INSIDE)
	{
		// If the renderer is fully contained within the child node's bounding box,
		// then it cannot intersect the other child nodes, so we don't check the other nodes
		return;
	}
	if (m_TopRight->AddRenderer(aRenderer) == AABBIntersectType::AABB_INSIDE)
	{
		return;
	}
	if (m_BottomLeft->AddRenderer(aRenderer) == AABBIntersectType::AABB_INSIDE)
	{
		return;
	}

	m_BottomRight->AddRenderer(aRenderer);
}

void QuadtreeNode::FrustumCull(Frustum aViewFrustum)
{

	Culled = false;
	//TEMPORARY - until QuadtreeNode bounding boxes are converted to 3D or a good way to check frustums with AABB2D is found
	//TEMPORARY - using -1000 to 1000 since numeric limits gives a false calculation at certain angles (overflow?)
	//AABB boundingBox3D(glm::vec3(m_BoundingBox.MinBounds.x, std::numeric_limits<float>::min(), m_BoundingBox.MinBounds.y), glm::vec3(m_BoundingBox.MaxBounds.x, std::numeric_limits<float>::max(), m_BoundingBox.MaxBounds.y));
	AABB boundingBox3D(glm::vec3(m_BoundingBox.MinBounds.x, -10, m_BoundingBox.MinBounds.y), glm::vec3(m_BoundingBox.MaxBounds.x, 10, m_BoundingBox.MaxBounds.y));


	unsigned int result = aViewFrustum.CheckIntersection(boundingBox3D);

	if (result == AABBIntersectType::AABB_OUTSIDE)
	{
		// Do not set any renderers as visible
		Culled = true;
		return;
	}
	else if (result == AABBIntersectType::AABB_INTERSECT)
	{
		if (m_IsLeafNode)
		{
			SetVisible();
		}
		else
		{
			m_BottomLeft->FrustumCull(aViewFrustum);
			m_BottomRight->FrustumCull(aViewFrustum);
			m_TopLeft->FrustumCull(aViewFrustum);
			m_TopRight->FrustumCull(aViewFrustum);
		}
	}
	else if (result == AABBIntersectType::AABB_INSIDE)
	{
		SetVisibleRecursive();
	}
}

void QuadtreeNode::SetVisible()
{
	for (int i = 0; i < m_Renderers.size(); ++i)
	{
		m_Renderers.at(i)->SetCulled(false);
	}
}

void QuadtreeNode::SetVisibleRecursive()
{
	if (m_IsLeafNode)
	{
		for (int i = 0; i < m_Renderers.size(); ++i)
		{
			m_Renderers.at(i)->SetCulled(false);
		}
	}
	else
	{
		m_BottomLeft->SetVisibleRecursive();
		m_BottomRight->SetVisibleRecursive();
		m_TopLeft->SetVisibleRecursive();
		m_TopRight->SetVisibleRecursive();
	}
}

void QuadtreeNode::DebugDraw(DebugRenderer* aRenderer)
{
	glm::vec2 distance = m_BoundingBox.MaxBounds - m_BoundingBox.MinBounds;
	glm::vec2 pos = m_BoundingBox.MinBounds + (distance / 2.f);

	glm::vec3 scale(distance.x, 1, distance.y);

	glm::vec4 color((m_Depth * 0.2f), 0.1f, 1.f - (m_Depth * 0.2f), 0.4f + (m_Depth * 0.1f));
	if (Culled) { color = glm::vec4(1, 1, 1, 1); }

	aRenderer->DrawPlane(glm::vec3(pos.x, -8 + m_Depth, pos.y), glm::quat(), scale, color);

	if (!m_IsLeafNode)
	{
		m_BottomLeft->DebugDraw(aRenderer);
		m_BottomRight->DebugDraw(aRenderer);
		m_TopLeft->DebugDraw(aRenderer);
		m_TopRight->DebugDraw(aRenderer);
	}
}