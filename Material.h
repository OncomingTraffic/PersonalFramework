#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>


class Shader;
class Renderer;


/*
Represents a material used to render objects
Has textures and shader uniforms
Objects that share the same material will render at the same time
*/
class Material
{
public:

	Material();
	virtual ~Material();

	virtual bool Initialize();

	virtual void SetupAttributes();
	virtual void SetupUniforms(Renderer* renderer); //Switched to raw pointers to make it easier to use "this"

	std::shared_ptr<Shader> GetShader();

protected:

	// The shader used to render this material
	std::shared_ptr<Shader> m_Shader;

};


#endif //MATERIAL_H