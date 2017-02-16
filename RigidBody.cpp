#include "Rigidbody.h"
#include "PhysicsManager.h"
#include "../src/Core/GameObject.h"
#include <SDL.h>

RigidBody::RigidBody() : m_Mass(0)
{
	//
}

RigidBody::RigidBody(PhysicsManager* aPhysicsManager) : m_PhysicsManager(aPhysicsManager), m_Mass(0)
{
	//
}

RigidBody::~RigidBody()
{
	if (m_PhysicsManager != nullptr && m_RigidBody != nullptr)
	{
		m_PhysicsManager->RemoveRigidbody(m_RigidBody.get());
	}
}


bool RigidBody::Initialize()
{
	if (!m_CollisionShape)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Rigidbody does not have collision shape\n");
		return false;
	}

	if (!m_PhysicsManager)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Rigidbody has no reference to PhysicsManager\n");
		return false;
	}
	//m_RigidBody = std::make_unique<btRigidBody>();
	//btStaticPlaneShape
	//btRigidBody()
	btTransform transform;
	transform.setIdentity();
	glm::vec3 pos = m_Parent->GetPosition();
	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	m_MotionState = std::make_unique<btDefaultMotionState>(transform);

	//(mass, myMotionState, groundShape, localInertia);
	m_RigidBody = std::make_unique<btRigidBody>(m_Mass, m_MotionState.get(), m_CollisionShape.get(), btVector3(0, 0, 0));
	
	m_PhysicsManager->AddRigidbody(m_RigidBody.get());

	
	
	// Needs reference to parent to get transforms
	// Needs reference to mesh in case of Convex Hull or Triangle Mesh collision shape

	return true;
}


void RigidBody::UpdateParentPosition()
{
	if (true) // TODO: Only update when RigidBody is active
	{
		//if (m_RigidBody->isActive())

		m_Parent->SetPosition(ToGlmVec3(m_RigidBody->getWorldTransform().getOrigin()));
	}
}

void RigidBody::UpdateRigidBodyPosition()
{
	if (true) // TODO: Only update when Parent transform changes
	{
		//m_RigidBody->

		btTransform transform;
		m_MotionState->getWorldTransform(transform);
		transform.setOrigin(ToBulletVec3(m_Parent->GetPosition()));
	}
}


std::unique_ptr<btRigidBody>& RigidBody::GetRigidBody()
{
	return m_RigidBody;
}

void RigidBody::SetMass(float aMass)
{
	m_Mass = aMass;
}

float RigidBody::GetMass()
{
	return m_Mass;
}

void RigidBody::SetCollisionShape(std::shared_ptr<btCollisionShape> aCollisionShape)
{
	m_CollisionShape = aCollisionShape;
}

std::shared_ptr<btCollisionShape> RigidBody::GetCollisionShape()
{
	return m_CollisionShape;
}

void RigidBody::SetPhysicsManager(PhysicsManager* aPhysicsManager)
{
	if (m_IsLoaded && m_PhysicsManager != nullptr)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "PhysicsManager is already set. Rigidbody may be part of a PhysicsWorld already");
	}

	m_PhysicsManager = aPhysicsManager;
}

PhysicsManager* RigidBody::GetPhysicsManager()
{
	return m_PhysicsManager;
}



glm::vec3 ToGlmVec3(const btVector3& aBulletVector)
{
	return glm::vec3(aBulletVector.getX(), aBulletVector.getY(), aBulletVector.getZ());
}

btVector3 ToBulletVec3(const glm::vec3& aGlmVector)
{
	return btVector3(aGlmVector.x, aGlmVector.y, aGlmVector.z);
}

std::string RigidBody::GetType()
{
	return std::string("RigidBody");
}