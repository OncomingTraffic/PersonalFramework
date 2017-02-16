#ifndef AABB2D_H
#define AABB2D_H

#include <GL\glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>
#include "AABB.h"


enum AABBIntersectType
{
	// The bounding boxes do not intersect
	AABB_OUTSIDE = 0,

	// The bounding boxes intersect
	AABB_INTERSECT = 1,

	// The second box is fully contained within the first
	AABB_INSIDE = 2
};

// Axis Aligned Bounding Box in 2 dimensions
struct AABB2D
{
	//TODO: Maybe change to center and half extents
	// This may be faster for intersects check

	// Minimum bounds
	glm::vec2 MinBounds;

	// Maximum bounds
	glm::vec2 MaxBounds;

	AABB2D();
	AABB2D(glm::vec2 minBounds, glm::vec2 maxBounds);
	AABB2D(float x, float y, float width, float height);

	// Check if bounding box overlaps this one
	bool Intersects(AABB2D &boundingBox);

	// Check if this bounding box intersects a 3D bounding box
	// Compares 2D y dimensions to the 3D z dimensions
	bool Intersects(AABB &boundingBox);

	// Check if this bounding box intersects a 3D bounding box
	// Compares 2D y dimensions to the 3D y dimensions
	bool IntersectsVertical(AABB &boundingBox);

	// Check if a bounding box is fully contained within this one
	bool Contains(AABB2D &boundingBox);

	// Check if two bounding boxes intersect
	// Returns the type of intersection that occurs
	AABBIntersectType CheckIntersection(AABB2D &boundingBox);

	// Check if this bounding box intersects a 3D bounding box
	// Compares 2D y dimensions to the 3D z dimensions
	// Returns the type of intersection that occurs
	AABBIntersectType CheckIntersection(AABB &boundingBox);

	
};



#endif //AABB2D_H