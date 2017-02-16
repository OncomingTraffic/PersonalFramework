#include "Core/Mesh.h"
#include <SDL.h>
#include "ObjLoader.h"

Mesh::Mesh() : m_VBO(0), m_IBO(0), m_VertexCount(0), m_IndexCount(0)
{
	//
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}


bool Mesh::LoadMesh(std::string aFilePath)
{
	// Generate a new vertex buffer if there isn't one already
	if (m_VBO == 0)
	{
		glGenBuffers(1, &m_VBO);
	}
	// Generate a new index buffer if there isn't one already
	if (m_IBO == 0)
	{
		glGenBuffers(1, &m_IBO);
	}
	// Will return false if the OBJ fails to load
	return LoadOBJ(aFilePath.c_str(), m_VBO, m_VertexCount, m_IBO, m_IndexCount);
}

void Mesh::BindArrayBuffers()
{
	//TODO: Switch to using Vertex Array Objects (VAO) instead
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

const GLuint& Mesh::GetIndexCount()
{
	return m_IndexCount;
}