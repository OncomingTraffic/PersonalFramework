#ifndef AABB_H
#define AABB_H

#include <GL\glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>


// Axis Aligned Bounding Box
struct AABB
{
	//TODO: Maybe change to center and half extents
	// This may be faster for intersects check

	// Minimum bounds
	glm::vec3 MinBounds;

	// Maximum bounds
	glm::vec3 MaxBounds;

	AABB();
	AABB(glm::vec3 minBounds, glm::vec3 maxBounds);
	AABB(float x, float y, float z, float width, float height, float depth);

	bool Intersects(AABB &boundingBox);
	bool Contains(AABB &boundingBox);

	// Get the max vertex relative to a normal
	glm::vec3 GetPositiveVertex(glm::vec3 normal);

	// Get the min vertex relative to a normal
	glm::vec3 GetNegativeVertex(glm::vec3 normal);
};



#endif //AABB_H