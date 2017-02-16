#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <vector>
#include <string>

class Camera;

class CameraManager
{
	// A list of cameras
	std::vector<Camera*> m_Cameras;

	// The cuurently active camera, this is the camera that objects use to render
	Camera* m_ActiveCamera;

public:
	CameraManager();
	~CameraManager();

	// Add a new camera to the list, makeActive will make the added camera the active one
	void AddCamera(Camera* camera, bool makeActive = false);

	// Remove a camera from the list, a new active camera will be chosen if the current active one is removed
	void RemoveCamera(std::string cameraTag);

	// Set a new camera, using the camera's tag to identify it
	void SetActiveCamera(std::string cameraTag);

	// Returns the currently active camera
	Camera* GetActiveCamera();
};


#endif CAMERAMANAGER_H