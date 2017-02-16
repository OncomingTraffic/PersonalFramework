#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include <string>
#include "Component.h"
#include "../Frustum.h"


class BaseObject;

class Camera : public Component
{
public:
	Camera();
	Camera(std::string tag, float zoom = 1.0f);
	~Camera();

	virtual bool Initialize();

	virtual void Cleanup();

	void Update(float delta);

	// Move the camera forward relative to its orientation, pass a negative value to move backwards
	void MoveForward(float distance);

	// Move the camera upward relative to its orientation, pass a negative value to move downwards
	void MoveUp(float distance);

	// Move the camera right relative to its orientation, pass a negative value to move left
	void MoveRight(float distance);

	void SetZoom(float zoom);
	float GetZoom();

	// Orient the camera to face a specific point, currently always has the camera upright (roll of 0)
	void LookAtTarget(glm::vec3 position);

	// Calculate the view-projection matrix used to render objects
	void CalculateViewProjectionMatrix();

	// Calculate the forward, up, and right vectors
	void CalculateDirectionVectors();

	// Get the view-projection matrix used to render objects
	glm::mat4 GetViewProjectionMatrix();

	Frustum GetViewFrustum();

	// Get the tag used to identify this camera
	std::string GetTag();

	std::string GetType();

	// A vector going upwards from the camera
	glm::vec3 m_UpVector;

	// A vector going in front of the camera
	glm::vec3 m_ForwardVector;

	// A vector going right from the camera
	glm::vec3 m_RightVector;


protected:

	// A tag used to identify this camera
	std::string m_Tag;

	// The zoom factor of the camera view
	float m_Zoom;

	// The view projection matrix used to orient and render objects
	glm::mat4 m_ViewProjMatrix;

	// Whether or not m_ViewProjMatrix should be recalculated
	bool m_ViewProjIsDirty;

	Frustum m_ViewFrustum;
};


#endif CAMERA_H