#include "PhysicsObject.h"
#include <SDL.h>
#include "../PhysicsManager.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

PhysicsObject::PhysicsObject() : BaseObject(), m_CollisionObjectType(CollisionObjectType::CO_RIGIDBODY)
{
	//
}

PhysicsObject::PhysicsObject(glm::vec3 aPosition, glm::vec3 aRotation, glm::vec3 aScale, float aMass, glm::vec3 aVelocity, glm::vec3 aAngularVelocity, bool aIsEnabled) : BaseObject()
{
	//
}

PhysicsObject::~PhysicsObject()
{
	if (m_Initialized)
	{
		Cleanup();
	}
	//m_PhysicsManager->RemoveRigidbody((btRigidBody*)m_CollisionObject.get());
}


bool PhysicsObject::Initialize()
{
	if (!m_CollisionObject)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "PhysicsObject has no collision object");
		return false;
	}

	if (!m_PhysicsManager)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "PhysicsObject has no reference to PhysicsManager");
		return false;
	}

	//(mass, myMotionState, groundShape, localInertia);
	//m_RigidBody = std::make_unique<btRigidBody>(m_Mass, m_MotionState.get(), m_CollisionShape.get(), btVector3(0, 0, 0));

	//TODO: Allow PhysicsManager to accept all types of CollisionObjects and add a way to tell what type m_CollisionObject is
	m_PhysicsManager->AddRigidbody((btRigidBody*)(m_CollisionObject.get()));

	//TODO:
	// Needs reference to parent to get transforms
	// Needs reference to mesh in case of Convex Hull or Triangle Mesh collision shape

	m_Initialized = true;

	return true;
}

void PhysicsObject::Cleanup()
{
	BaseObject::Cleanup();
	
	m_PhysicsManager->RemoveRigidbody((btRigidBody*)m_CollisionObject.get());
}


void PhysicsObject::SetPosition(glm::vec3 aPosition)
{
	//TODO: Setting rigidbody positions requires either warps or for into to be removed
	//		and added back into the physics world in a new position
}

void PhysicsObject::SetPosition(btVector3 &aPosition)
{
}

void PhysicsObject::SetPositionX(float aPositionX)
{
}

void PhysicsObject::SetPositionY(float aPositionY)
{
}

void PhysicsObject::SetPositionZ(float aPositionZ)
{
}

glm::vec3 PhysicsObject::GetPosition()
{
	btVector3 bulletPos = m_CollisionObject->getWorldTransform().getOrigin();

	return glm::vec3(bulletPos.getX(), bulletPos.getY(), bulletPos.getZ());
}

btVector3 PhysicsObject::GetBTPosition()
{
	return m_CollisionObject->getWorldTransform().getOrigin();
}


void PhysicsObject::SetRotation(glm::quat aRotation)
{

}

void PhysicsObject::SetRotation(glm::vec3 aEulerAngles)
{

}

void PhysicsObject::SetRotation(btQuaternion &aRotation)
{
	//
}

void PhysicsObject::Rotate(float aAngle, glm::vec3 aAxis)
{

}

glm::quat PhysicsObject::GetRotation()
{
	btQuaternion bulletQuat = m_CollisionObject->getWorldTransform().getRotation();

	// W is first in the quat constructor
	return glm::quat(bulletQuat.getW(), bulletQuat.getX(), bulletQuat.getY(), bulletQuat.getZ());
}

btQuaternion PhysicsObject::GetBTRotation()
{
	return m_CollisionObject->getWorldTransform().getRotation();
}


void PhysicsObject::SetScale(glm::vec3 aScale)
{
}

void PhysicsObject::SetScale(btVector3 &aScale)
{
}

void PhysicsObject::SetScaleX(float aScaleX)
{
}

void PhysicsObject::SetScaleY(float aScaleY)
{
}

void PhysicsObject::SetScaleZ(float aScaleZ)
{
}

glm::vec3 PhysicsObject::GetScale()
{
	btVector3 bulletScale = m_CollisionObject->getCollisionShape()->getLocalScaling();

	return glm::vec3(bulletScale.getX(), bulletScale.getY(), bulletScale.getZ());
}

btVector3 PhysicsObject::GetBTScale()
{
	return m_CollisionObject->getCollisionShape()->getLocalScaling();
}

glm::mat4x4 PhysicsObject::GetModelMatrix()
{
	//TODO: Store this matrix so it doesn't need to be recalculated every time
	glm::mat4x4 matrix; // Scale, then Rotate, then Translate

	//TODO: MotionState should be used wherever possible
	//m_CollisionObject->getWorldTransform().getOpenGLMatrix(&matrix[0][0]);

	btTransform t;
	m_MotionState->getWorldTransform(t);
	
	t.getOpenGLMatrix(&matrix[0][0]);
	//t.getOpenGLMatrix(glm::value_ptr(matrix)); // Not sure if this works better

	//TEMPORARY - this will change the scale (not sure if properly), but physics stop checking after object deactivates (sleeps)
	btVector3 bulletScale = m_CollisionObject->getCollisionShape()->getLocalScaling();

	matrix *= glm::scale(glm::vec3(bulletScale.getX(), bulletScale.getY(), bulletScale.getZ()));// Is this the right order?

	return matrix;
}

void PhysicsObject::SetPhysicsManager(PhysicsManager* aPhysicsManager)
{
	m_PhysicsManager = aPhysicsManager;
}

void PhysicsObject::SetCollisionObject(std::shared_ptr<btCollisionObject> collisionObject)
{
	//
}

std::shared_ptr<btCollisionObject> PhysicsObject::GetCollisionObject()
{
	return m_CollisionObject;
}

void PhysicsObject::SetRigidbody(const float &aMass, std::shared_ptr<btCollisionShape> aCollisionShape, const btVector3 &aLocalInertia)
{
	if (!aCollisionShape)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Rigidbody does not have collision shape");
		return;
	}

	m_CollisionShape = aCollisionShape;

	btTransform transform;
	transform.setIdentity();

	m_MotionState = std::make_unique<btDefaultMotionState>(transform);

	//(mass, myMotionState, collisionShape, localInertia);
	m_CollisionObject = std::make_unique<btRigidBody>(aMass, m_MotionState.get(), m_CollisionShape.get(), aLocalInertia);
}

void PhysicsObject::SetRigidbody(const float &aMass, std::shared_ptr<btCollisionShape> aCollisionShape, const btTransform &aInitialTranform, const btVector3 &aLocalInertia)
{
	if (!aCollisionShape)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Rigidbody does not have collision shape");
		return;
	}

	m_CollisionShape = aCollisionShape;

	m_MotionState = std::make_unique<btDefaultMotionState>(aInitialTranform);

	//(mass, myMotionState, collisionShape, localInertia);
	m_CollisionObject = std::make_unique<btRigidBody>(aMass, m_MotionState.get(), m_CollisionShape.get(), aLocalInertia);

	m_CollisionObjectType = CollisionObjectType::CO_RIGIDBODY;
}

btRigidBody* PhysicsObject::GetRigidbody()
{
	if (m_CollisionObjectType == CollisionObjectType::CO_RIGIDBODY)
	{
		return (btRigidBody*)m_CollisionObject.get();
	}

	return nullptr;
}

btMotionState* PhysicsObject::GetMotionState()
{
	return m_MotionState.get();
}