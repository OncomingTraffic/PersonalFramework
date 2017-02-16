#include "Frustum.h"
#include "AABB2D.h"


Plane::Plane() : normal(glm::vec3()), distance(0)
{

}
Plane::Plane(glm::vec4 aDimensions) : normal(aDimensions.x, aDimensions.y, aDimensions.z), distance(aDimensions.w)
{
	//
}

void Plane::Normalize()
{
	float magnitude = glm::length(normal);
	normal /= magnitude;
	distance /= magnitude;
}


Frustum::Frustum()
{
	//
}

Frustum::Frustum(glm::mat4x4 aMatrix)
{
	//TODO: Make sure this is correct
	planes[0] = Plane(aMatrix[3] + aMatrix[0]); // Left
	planes[1] = Plane(aMatrix[3] - aMatrix[0]); // Right
	planes[2] = Plane(aMatrix[3] - aMatrix[1]); // Top
	planes[3] = Plane(aMatrix[3] + aMatrix[1]); // Bottom
	planes[4] = Plane(aMatrix[3] + aMatrix[2]); // Near
	planes[5] = Plane(aMatrix[3] - aMatrix[2]); // Far

	planes[0].Normalize();
	planes[1].Normalize();
	planes[2].Normalize();
	planes[3].Normalize();
	planes[4].Normalize();
	planes[5].Normalize();
}

// Sets the planes of this frustum using a 4x4 matrix
void Frustum::CreateFromMatrix(glm::mat4x4 aMatrix)
{
	planes[0] = Plane(aMatrix[3] + aMatrix[0]); // Left
	planes[1] = Plane(aMatrix[3] - aMatrix[0]); // Right
	planes[2] = Plane(aMatrix[3] - aMatrix[1]); // Top
	planes[3] = Plane(aMatrix[3] + aMatrix[1]); // Bottom
	planes[4] = Plane(aMatrix[3] + aMatrix[2]); // Near
	planes[5] = Plane(aMatrix[3] - aMatrix[2]); // Far

	if (0)
	{
		planes[0].normal.x = aMatrix[0].w + aMatrix[0].x;
		planes[0].normal.y = aMatrix[1].w + aMatrix[1].x;
		planes[0].normal.z = aMatrix[2].w + aMatrix[2].x;
		planes[0].distance = aMatrix[3].w + aMatrix[3].x;

		planes[1].normal.x = aMatrix[0].w - aMatrix[0].x;
		planes[1].normal.y = aMatrix[1].w - aMatrix[1].x;
		planes[1].normal.z = aMatrix[2].w - aMatrix[2].x;
		planes[1].distance = aMatrix[3].w - aMatrix[3].x;

		planes[2].normal.x = aMatrix[0].w - aMatrix[0].y;
		planes[2].normal.y = aMatrix[1].w - aMatrix[1].y;
		planes[2].normal.z = aMatrix[2].w - aMatrix[2].y;
		planes[2].distance = aMatrix[3].w - aMatrix[3].y;

		planes[3].normal.x = aMatrix[0].w + aMatrix[0].y;
		planes[3].normal.y = aMatrix[1].w + aMatrix[1].y;
		planes[3].normal.z = aMatrix[2].w + aMatrix[2].y;
		planes[3].distance = aMatrix[3].w + aMatrix[3].y;

		planes[4].normal.x = aMatrix[0].w + aMatrix[0].z;
		planes[4].normal.y = aMatrix[1].w + aMatrix[1].z;
		planes[4].normal.z = aMatrix[2].w + aMatrix[2].z;
		planes[4].distance = aMatrix[3].w + aMatrix[3].z;

		planes[5].normal.x = aMatrix[0].w - aMatrix[0].z;
		planes[5].normal.y = aMatrix[1].w - aMatrix[1].z;
		planes[5].normal.z = aMatrix[2].w - aMatrix[2].z;
		planes[5].distance = aMatrix[3].w - aMatrix[3].z;
	}

	//planes[0] = Plane(glm::vec4(aMatrix[3][0] + aMatrix[0][0], aMatrix[3][1] + aMatrix[0][1], aMatrix[3][2] + aMatrix[0][2], aMatrix[3][3] + aMatrix[0][3]));
	//planes[1] = Plane(glm::vec4(aMatrix[3][0] - aMatrix[0][0], aMatrix[3][1] - aMatrix[0][1], aMatrix[3][2] - aMatrix[0][2], aMatrix[3][3] - aMatrix[0][3]));
	//planes[2] = Plane(glm::vec4(aMatrix[3][0] - aMatrix[1][0], aMatrix[3][1] - aMatrix[1][1], aMatrix[3][2] - aMatrix[1][2], aMatrix[3][3] - aMatrix[1][3]));
	//planes[3] = Plane(glm::vec4(aMatrix[3][0] + aMatrix[1][0], aMatrix[3][1] + aMatrix[1][1], aMatrix[3][2] + aMatrix[1][2], aMatrix[3][3] + aMatrix[1][3]));
	//planes[4] = Plane(glm::vec4(aMatrix[3][0] + aMatrix[2][0], aMatrix[3][1] + aMatrix[2][1], aMatrix[3][2] + aMatrix[2][2], aMatrix[3][3] + aMatrix[2][3]));
	//planes[5] = Plane(glm::vec4(aMatrix[3][0] - aMatrix[2][0], aMatrix[3][1] - aMatrix[2][1], aMatrix[3][2] - aMatrix[2][2], aMatrix[3][3] - aMatrix[2][3]));

	//planes[0] = Plane(glm::vec4(aMatrix[0][3] + aMatrix[0][0], aMatrix[1][3] + aMatrix[1][0], aMatrix[2][3] + aMatrix[2][0], aMatrix[3][3] + aMatrix[3][0]));
	//planes[1] = Plane(glm::vec4(aMatrix[0][3] - aMatrix[0][0], aMatrix[1][3] - aMatrix[1][0], aMatrix[2][3] - aMatrix[2][0], aMatrix[3][3] - aMatrix[3][0]));
	//planes[2] = Plane(glm::vec4(aMatrix[0][3] - aMatrix[0][1], aMatrix[1][3] - aMatrix[1][1], aMatrix[2][3] - aMatrix[2][1], aMatrix[3][3] - aMatrix[3][1]));
	//planes[3] = Plane(glm::vec4(aMatrix[0][3] + aMatrix[0][1], aMatrix[1][3] + aMatrix[1][1], aMatrix[2][3] + aMatrix[2][1], aMatrix[3][3] + aMatrix[3][1]));
	//planes[4] = Plane(glm::vec4(aMatrix[0][3] + aMatrix[0][2], aMatrix[1][3] + aMatrix[1][2], aMatrix[2][3] + aMatrix[2][2], aMatrix[3][3] + aMatrix[3][2]));
	//planes[5] = Plane(glm::vec4(aMatrix[0][3] - aMatrix[0][2], aMatrix[1][3] - aMatrix[1][2], aMatrix[2][3] - aMatrix[2][2], aMatrix[3][3] - aMatrix[3][2]));

	planes[0].Normalize();
	planes[1].Normalize();
	planes[2].Normalize();
	planes[3].Normalize();
	planes[4].Normalize();
	planes[5].Normalize();
}

unsigned int Frustum::CheckIntersection(AABB aBoundingBox)
{
	//glm::vec3 min = aBoundingBox.MinBounds;
	//glm::vec3 max = aBoundingBox.MaxBounds;
	//glm::vec3 points[8] = { min, glm::vec3(max.x, min.y, min.z), glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z),
	//						glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), max };

	// Using a quick AABB check, which may not be completely accurate. Some objects could pass the check when they shouldn't
	//for (int i = 0; i < 6; ++i)
	//{
	//	// If the nearest vertex is outside then it is not within the frustum
	//	if ((planes[i].normal.x * 0) + (planes[i].normal.y * 0) + (planes[i].normal.z * 0) + planes[i].distance < 0.0f)
	//	{
	//
	//		return AABB_OUTSIDE;
	//	}
	//}
	//return AABB_INSIDE;

	unsigned short intersectType = AABB_INSIDE;

	//TODO: This check fails for large bounding boxes
	// Check the bounding box against all six planes of the frustum
	for (int i = 0; i < 6; ++i)
	{
		glm::vec3 posVertex = aBoundingBox.GetPositiveVertex(planes[i].normal);
		glm::vec3 negVertex = aBoundingBox.GetNegativeVertex(planes[i].normal);

		// If the nearest vertex is outside then it is not within the frustum
		if (glm::dot(planes[i].normal, aBoundingBox.GetPositiveVertex(planes[i].normal)) + planes[i].distance < 0.0f)
		{
			return AABB_OUTSIDE;
		}

		// If the furthest vertex is outside, but the nearest is inside, then we are intersecting and need to check the other planes
		else if (glm::dot(planes[i].normal, aBoundingBox.GetNegativeVertex(planes[i].normal)) + planes[i].distance < 0.0f)
		{
			intersectType = AABB_INTERSECT;
		}
	}

	// Return the intersect type if it's AABB_INTERSECT or AABB_INSIDE
	return intersectType;
}