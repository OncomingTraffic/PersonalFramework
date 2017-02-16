#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <map>
#include <memory>

class Shader;

class ShaderManager
{
	// A list of all Shader Programs
	std::map<std::string, std::shared_ptr<Shader>> m_ShaderPrograms;

public:
	ShaderManager();
	~ShaderManager();

	// If the shader already exists, return it, otherwise load the shader then return it
	// This method assumes that the vertex and fragment shader have the same name and end in ".vert" and ".frag" respectively
	// TODO: Change this name to something more suitable
	std::shared_ptr<Shader> GetShader(std::string &shaderName);
	
	// Load a shader that uses different names for the vertex and fragment shaders, then return it
	// 'programName' represents the name used to reference this shader program in the list
	// This method requires you to specify the file extensions of the shader files you're loading
	//Shader* LoadShader(std::string &vertShaderName, std::string &fragShaderName, std::string &programName);

	void AddShader(std::string shaderName, std::shared_ptr<Shader> shader);

	// Close the shader and remove from the map of loaded shaders
	void CloseShader(std::string shaderName);

	// Iterate through the map and close and remove all shader programs
	void CloseAllShaders();
};


#endif SHADERMANAGER_H