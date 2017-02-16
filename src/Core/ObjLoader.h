#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <GL\glew.h>

/*
Description: Loads a .obj file to use as a mesh
Arguments:
	filePath	- the file to be loaded
	VBO			- a reference to the Vertex Buffer Object to store the vertices in
	vertexCount	- a reference to a variable to store the vertex count
	IBO			- a reference to the Index Buffer Object to store the vertices in
	indexCount	- a reference to a variable to store the index count
Returns: True if the object was successfully loaded, or false if anything went wrong
*/
bool LoadOBJ(const char* filePath, GLuint &VBO, unsigned int &vertexCount, GLuint &IBO, unsigned int &indexCount);

bool DisplayTextFile(const char* filePath);


#endif OBJLOADER_H