#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H


#include <vector>
#include <map>
#include <memory>
#include <GL\glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>
#include "src\Core\Shader.h"
#include "RenderingQuadtree.h"
#include "DebugRenderer.h"


class Renderer;
class Scene;
class Camera;
class PointLight;


struct ShaderComparator
{
	bool operator()(const std::shared_ptr<Shader> &a, const std::shared_ptr<Shader> &b) const
	{
		return a->GetRenderQueue() < b->GetRenderQueue();
	}
};



class RenderingManager
{
public:

	// The rendering manager should not be created outside of a scene
	RenderingManager(Scene* parentScene);
	~RenderingManager();

	void Draw();

	void AddShader(std::shared_ptr<Shader> shader);
	void AddRenderer(Renderer* renderer);

	void RemoveRenderer(Renderer* renderer);


	// Add a new camera to the list, makeActive will make the added camera the active one
	// Should only be called automatically by a Camera when it's created
	void AddCamera(Camera* camera, bool makeActive = false);

	// Remove a camera from the list, a new active camera will be chosen if the current active one is removed
	void RemoveCamera(std::string cameraTag);

	// Set a new camera, using the camera's tag to identify it
	void SetActiveCamera(std::string cameraTag);

	// Returns the currently active camera
	Camera* GetActiveCamera();

	// Set if the Rendering Manager should use spacial partitioning to render and cull,
	// or use a brute force mode and render everything
	void SetBruteForceRendering(bool useBruteForce);


	glm::vec3 GetAmbientColor();
	void SetAmbientColor(glm::vec3 color);

	glm::vec3 GetDirectionalLightColor();
	void SetDirectionalLightColor(glm::vec3 color);

	glm::vec3 GetDirectionalLightNormal();
	void SetDirectionalLightNormal(glm::vec3 normal);

	void AddPointLight(PointLight* light);
	void RemovePointLight(PointLight* light);

	//Calculate Nearby Point Lights
	std::vector<PointLight*> GetNearestLights(glm::vec3 position);	


private:

	std::map<std::shared_ptr<Shader>, std::vector<Renderer*>, ShaderComparator> m_RenderBatches;

	// The scene that this belongs to
	Scene* m_ParentScene;

	std::vector<Camera*> m_Cameras;

	Camera* m_ActiveCamera;

	//TODO: Allow different types of Spacial Partitioning
	std::unique_ptr<QuadtreeNode> m_Quadtree;

	std::unique_ptr<DebugRenderer> m_DebugRenderer;

	//Lights

	//TEMPORARY - maybe
	// The maximum amount of Point Lights the can affect an object in each render
	// This amount needs to match the amount specified in each shader that uses lighting
	unsigned short m_MaxPointLights;

	std::vector<PointLight*> m_PointLights;

	// The color of the ambient light, which effects the whole scene
	glm::vec3 m_AmbientColor;

	// The color of the directional light
	glm::vec3 m_DirectionalLightColor;

	// The normalized direction of the directional light
	glm::vec3 m_DirectionalLightNormal;


	// True if the Rendering Manager uses a spacial partioning tree to cull
	bool m_UseBruteForceRendering;

};




#endif //RENDERINGMANAGER_H