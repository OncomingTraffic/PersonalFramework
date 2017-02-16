#ifndef SHADERCOLOR_H
#define SHADERCOLOR_H

#include "src\Core\Shader.h"


class ShaderColor : public Shader
{
public:

	ShaderColor();
	virtual ~ShaderColor();

	bool Initialize();

	void SetupAttributes();

	void EnableGLCapabilities();
	void DisableGLCapabilities();

private:

	// Attributes
	GLint m_aPos;
	GLint m_aNormal;
	GLint m_aUV;
};



#endif //SHADERCOLOR_H