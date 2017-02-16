#include "Material.h"

Material::Material() : m_Shader(nullptr)
{
	//
}

Material::~Material()
{
	m_Shader = nullptr;
}


bool Material::Initialize()
{
	return true;
}

void Material::SetupAttributes()
{
	//
}

void Material::SetupUniforms(Renderer* aRenderer)
{
	//
}

std::shared_ptr<Shader> Material::GetShader()
{
	return m_Shader;
}