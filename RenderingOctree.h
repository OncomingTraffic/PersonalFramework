#ifndef RENDERINGOCTREE_H
#define RENDERINGOCTREE_H

#include <vector>
#include "AABB.h"

// Currently only MeshRenderers have Bounding Boxes
class MeshRenderer;

class OctreeNode
{
public:

	// Subdivide this node into eight new nodes
	void Split();

	void AddRenderer(MeshRenderer* object);


private:

	AABB m_BoundingBox;

	std::vector<OctreeNode> m_ChildNodes;

	std::vector<MeshRenderer*> m_Renderers;

	OctreeNode* m_ParentNode;
};




#endif RENDERINGOCTREE_H