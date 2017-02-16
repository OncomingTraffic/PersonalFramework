#ifndef RENDERINGQUADTREE_H
#define RENDERINGQUADTREE_H

#include <vector>
#include <memory>
#include <GL\glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>
#include "AABB2D.h"
#include "Frustum.h"


// Currently only MeshRenderers have Bounding Boxes
class MeshRenderer;
class RenderingManager;
class DebugRenderer;

// 
class QuadtreeNode
{
public:

	QuadtreeNode(unsigned short depth, unsigned short maxDepth, unsigned short maxRenderers, glm::vec2 AABBMin, glm::vec2 AABBMax, QuadtreeNode* parent);
	~QuadtreeNode();

	// Subdivide this node into four new nodes
	void Split();

	// Add a renderer to this nodes
	// Returns:
	//	0	the object is outside this node's bounds. The renderer was not added
	//	1	the object intersects this node's bounds and was added
	//	2	the object is contained fully in this node's bounds and was added
	unsigned short AddRenderer(MeshRenderer* renderer);

	// Attempt to add the renderer to the four child nodes
	void AddRendererToChildNodes(MeshRenderer* renderer);

	//TODO: Test bounding box against camera frustum and render
	void FrustumCull(Frustum viewFrustum);

	// Sets all renderers in this node as visible
	void SetVisible();

	// Sets all renderers in this node and all child nodes as visible
	void SetVisibleRecursive();

	void DebugDraw(DebugRenderer* renderer);


private:

	AABB2D m_BoundingBox;

	std::unique_ptr<QuadtreeNode> m_TopLeft;
	std::unique_ptr<QuadtreeNode> m_TopRight;
	std::unique_ptr<QuadtreeNode> m_BottomLeft;
	std::unique_ptr<QuadtreeNode> m_BottomRight;

	std::vector<MeshRenderer*> m_Renderers;

	// The owner node, one iteration up the tree
	QuadtreeNode* m_ParentNode;

	// The depth of this node, ie. how many iterations this node is down the Quadtree
	unsigned short m_Depth;

	// The Quadtree won't split anymore once it reaches this depth
	unsigned short m_MaxDepth;

	// The amount of renderers a node can have before splitting
	unsigned short m_MaxRenderersPerNode;

	// A lead node has no child nodes and stores a list of renderers
	bool m_IsLeafNode;

	//TEMPORARY
	bool Culled;
};



#endif //RENDERINGQUADTREE_H