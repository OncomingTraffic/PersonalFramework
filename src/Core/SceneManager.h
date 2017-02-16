#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>

class Scene;

/*
A class used to handle scenes.
It handles the updating and drawing of the active scene,
as well as transitions between active scenes.
*/
class SceneManager
{
	// A list of all scene
	std::vector<Scene*> m_Scenes;

	// The currently active scene
	Scene* m_ActiveScene;

public:
	SceneManager();
	~SceneManager();

	// Updates the active scene
	void Update(float delta);

	// Draws the active scene
	void Draw();

	// Add a new scene to the vector, makeActive determines if it should be set as the active scene
	void AddScene(Scene* scene, bool makeActive = false);

	// Removes a scene from the list. If it is the active scene, then you can set a new active scene
	void RemoveScene(std::string sceneTag, std::string newSceneTag = "");

	// Sets the active scene, using the scene's tag to identify it
	void SetActiveScene(std::string sceneTag);

	// Removes all scenes other than the active one
	void RemoveInactiveScenes();

	// Load an unloaded scene
	bool LoadScene(Scene* scene);

	// Returns the currently active scene
	Scene* GetActiveScene();
};

#endif SCENEMANAGER_H