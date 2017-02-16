#include "AABB2D.h"

AABB2D::AABB2D()
{
	//
}

AABB2D::AABB2D(glm::vec2 aMinBounds, glm::vec2 aMaxBounds) : MinBounds(aMinBounds), MaxBounds(aMaxBounds)
{
	//
}

AABB2D::AABB2D(float aX, float aY, float aWidth, float aHeight) : MinBounds(aX, aY), MaxBounds(aX + aWidth, aY + aHeight)
{
	//
}


bool AABB2D::Intersects(AABB2D &aBoundingBox)
{
	// Return false if any side can't intersect (ex. left side of box A is to the right of right side of box B)
	return !(aBoundingBox.MaxBounds.x < MinBounds.x || aBoundingBox.MinBounds.x > MaxBounds.x ||
		aBoundingBox.MaxBounds.y < MinBounds.y || aBoundingBox.MinBounds.y > MaxBounds.y);
}

bool AABB2D::Intersects(AABB &aBoundingBox)
{
	return !(aBoundingBox.MaxBounds.x < MinBounds.x || aBoundingBox.MinBounds.x > MaxBounds.x ||
		aBoundingBox.MaxBounds.z < MinBounds.y || aBoundingBox.MinBounds.z > MaxBounds.y);
}

bool AABB2D::IntersectsVertical(AABB &aBoundingBox)
{
	return !(aBoundingBox.MaxBounds.x < MinBounds.x || aBoundingBox.MinBounds.x > MaxBounds.x ||
		aBoundingBox.MaxBounds.y < MinBounds.y || aBoundingBox.MinBounds.y > MaxBounds.y);
}

bool AABB2D::Contains(AABB2D &aBoundingBox)
{
	return aBoundingBox.MaxBounds.x <= MaxBounds.x && aBoundingBox.MinBounds.x >= MinBounds.x &&
		aBoundingBox.MaxBounds.y <= MaxBounds.y && aBoundingBox.MinBounds.y >= MinBounds.y;
}

AABBIntersectType AABB2D::CheckIntersection(AABB2D &aBoundingBox)
{
	if (aBoundingBox.MaxBounds.x <= MaxBounds.x && aBoundingBox.MinBounds.x >= MinBounds.x &&
		aBoundingBox.MaxBounds.y <= MaxBounds.y && aBoundingBox.MinBounds.y >= MinBounds.y)
	{
		return AABBIntersectType::AABB_INSIDE;
	}
	else if (aBoundingBox.MaxBounds.x < MinBounds.x || aBoundingBox.MinBounds.x > MaxBounds.x ||
		aBoundingBox.MaxBounds.y < MinBounds.y || aBoundingBox.MinBounds.y > MaxBounds.y)
	{
		return AABBIntersectType::AABB_OUTSIDE;
	}
	else
	{
		return AABBIntersectType::AABB_INTERSECT;
	}
}

AABBIntersectType AABB2D::CheckIntersection(AABB &aBoundingBox)
{
	if (aBoundingBox.MaxBounds.x <= MaxBounds.x && aBoundingBox.MinBounds.x >= MinBounds.x &&
		aBoundingBox.MaxBounds.z <= MaxBounds.y && aBoundingBox.MinBounds.z >= MinBounds.y)
	{
		return AABBIntersectType::AABB_INSIDE;
	}
	else if (aBoundingBox.MaxBounds.x < MinBounds.x || aBoundingBox.MinBounds.x > MaxBounds.x ||
		aBoundingBox.MaxBounds.z < MinBounds.y || aBoundingBox.MinBounds.z > MaxBounds.y)
	{
		return AABBIntersectType::AABB_OUTSIDE;
	}
	else
	{
		return AABBIntersectType::AABB_INTERSECT;
	}
}