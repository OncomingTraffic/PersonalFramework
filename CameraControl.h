#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

#include "Behaviour.h"


class Camera;


class CameraControl : public Behaviour
{
public:

	CameraControl(Camera* camera);
	~CameraControl();

	void Update(float delta);

	void Cleanup();

	std::string GetType();

private:

	// Move the camera forward relative to its orientation, pass a negative value to move backwards
	void MoveForward(float distance);

	// Move the camera upward relative to its orientation, pass a negative value to move downwards
	void MoveUp(float distance);

	// Move the camera right relative to its orientation, pass a negative value to move left
	void MoveRight(float distance);

	void HandleRotation(float delta);

	// The camera that this behaviour controls
	Camera* m_Camera;

	float m_Yaw;
	float m_Pitch;

	bool m_LockMouse;

	int m_LockMouseX;
	int m_LockMouseY;
};



#endif //CAMERACONTROL_H