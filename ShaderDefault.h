#ifndef SHADERDEFAULT_H
#define SHADERDEFAULT_H

#include "src\Core\Shader.h"


class ShaderDefault : public Shader
{
public:

	ShaderDefault();
	virtual ~ShaderDefault();

	bool Initialize();

	void SetupAttributes();

	// Setup uniforms that are shared by every object rendering with this shader
	void SetupSharedUniforms(RenderingManager* renderingManager);

	void EnableGLCapabilities();
	void DisableGLCapabilities();

private:

	// Attributes
	GLint m_aPos;
	GLint m_aNormal;
	GLint m_aUV;

	// Shared Uniforms
	GLint m_uCameraPos;
	GLint m_uAmbientColor;
	GLint m_uDirectionalColor;
	GLint m_uDirectionalNormal;
};



#endif //SHADERDEFAULT_H