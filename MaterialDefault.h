#ifndef MATERIALDEFAULT_H
#define MATERIALDEFAULT_H

#include "Material.h"

#include <GL\glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>


class Texture;

//TODO: Find a way to have only one material class
//		Give materials a list of uniforms to set,
//		which is obtained from the shader they are using

class MaterialDefault : public Material
{
public:

	MaterialDefault();
	virtual ~MaterialDefault();

	virtual bool Initialize();

	virtual void SetupAttributes();
	virtual void SetupUniforms(Renderer* renderer);

protected:

	// Attributes
	GLint m_aPos;
	GLint m_aNormal;
	GLint m_aUV;

	//Uniforms
	GLint m_uTex;
	GLint m_uWVP;

	std::shared_ptr<Texture> m_Texture;

};


#endif //MATERIALDEFAULT_H