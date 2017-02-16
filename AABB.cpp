#include "AABB.h"

AABB::AABB()
{
	//
}

AABB::AABB(glm::vec3 aMinBounds, glm::vec3 aMaxBounds) : MinBounds(aMinBounds), MaxBounds(aMaxBounds)
{
	//
}

AABB::AABB(float aX, float aY, float aZ, float aWidth, float aHeight, float aDepth) : MinBounds(aX, aY, aZ), MaxBounds(aX + aWidth, aY + aHeight, aZ + aDepth)
{
	//
}


bool AABB::Intersects(AABB &aBoundingBox)
{
	// Return false if any side can't intersect (ex. left side of box A is to the right of right side of box B)
	return !(aBoundingBox.MaxBounds.x < MinBounds.x || aBoundingBox.MinBounds.x > MaxBounds.x ||
		aBoundingBox.MaxBounds.y < MinBounds.y || aBoundingBox.MinBounds.y > MaxBounds.y ||
		aBoundingBox.MaxBounds.z < MinBounds.z || aBoundingBox.MinBounds.z > MaxBounds.z);
}

bool AABB::Contains(AABB &aBoundingBox)
{
	return aBoundingBox.MaxBounds.x <= MaxBounds.x && aBoundingBox.MinBounds.x >= MinBounds.x &&
		aBoundingBox.MaxBounds.y <= MaxBounds.y && aBoundingBox.MinBounds.y >= MinBounds.y &&
		aBoundingBox.MaxBounds.z <= MaxBounds.z && aBoundingBox.MinBounds.z >= MinBounds.z;
}

glm::vec3 AABB::GetPositiveVertex(glm::vec3 aNormal)
{
	glm::vec3 point = MinBounds;

	if (aNormal.x > 0)
	{
		point.x = MaxBounds.x;
	}
	if (aNormal.y > 0)
	{
		point.y = MaxBounds.y;
	}
	if (aNormal.z > 0)
	{
		point.z = MaxBounds.z;
	}

	return point;
}

glm::vec3 AABB::GetNegativeVertex(glm::vec3 aNormal)
{
	glm::vec3 point = MaxBounds;

	if (aNormal.x > 0)
	{
		point.x = MinBounds.x;
	}
	if (aNormal.y > 0)
	{
		point.y = MinBounds.y;
	}
	if (aNormal.z > 0)
	{
		point.z = MinBounds.z;
	}

	return point;
}