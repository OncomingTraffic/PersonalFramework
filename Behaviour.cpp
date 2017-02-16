#include "Behaviour.h"
#include "BaseObject.h"
#include "src\Core\Scene.h"


Behaviour::Behaviour() : Component()
{
	//
}

Behaviour::~Behaviour()
{
	//
}


bool Behaviour::Initialize()
{
	if (m_Parent != nullptr)
	{
		m_Parent->GetParentScene()->AddBehaviour(this);

		return true;
	}

	return false;
}

void Behaviour::Cleanup()
{
	if (m_Parent != nullptr)
	{
		m_Parent->GetParentScene()->RemoveBehaviour(this);
	}
}

void Behaviour::Update(float aDelta)
{
	//
}