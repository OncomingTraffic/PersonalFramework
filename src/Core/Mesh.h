#ifndef MESH_H
#define MESH_H

#include <GL\glew.h>
#include <gl\GLU.h>
#include <string>

class ShaderProgram;

class Mesh
{
public:

	Mesh();
	virtual ~Mesh();

	virtual bool LoadMesh(std::string filePath);

	// Sets the VBO and IBO for rendering this object
	virtual void BindArrayBuffers();

	const GLuint& GetIndexCount();


protected:
	// A handle for the vertex array buffer used by this object when rendering
	GLuint m_VBO;

	// A handle for the index array buffer used by this object when rendering
	GLuint m_IBO;

	// The number of vertices used to render this object
	unsigned int m_VertexCount;

	// The number of indices used to render this object
	unsigned int m_IndexCount;

	//TODO: Store the vertex array if necessary

};


#endif MESH_H