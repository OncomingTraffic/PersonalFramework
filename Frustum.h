#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <GL\glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>
#include "AABB.h"

struct Plane
{
	Plane();
	Plane(glm::vec4 dimensions);

	void Normalize();


	glm::vec3 normal;
	float distance; // The offset from origin

};

struct Frustum
{
	Frustum();

	// Construct a frustum from a 4x4 matrix
	Frustum(glm::mat4x4 matrix);

	// Sets the planes of this frustum using a 4x4 matrix
	void CreateFromMatrix(glm::mat4x4 matrix);

	unsigned int CheckIntersection(AABB boundingBox);


	Plane planes[6];

};




#endif //FRUSTUM_H