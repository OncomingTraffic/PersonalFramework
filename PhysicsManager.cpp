#include "PhysicsManager.h"
#include <SDL.h>


PhysicsManager::PhysicsManager()
{
	//
}

PhysicsManager::~PhysicsManager()
{
	// Delete Rigidbodies

	if (m_PhysicsWorld->getNumCollisionObjects() > 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Physics Manager Warning!", "There are still Collision Objects in the Physics World.\nThis may cause memory leaks.", NULL);
	}

	m_PhysicsWorld.reset();

	m_ConstraintSolver.reset();

	m_BroadphaseInterface.reset();

	m_CollisionDispatcher.reset();

	m_CollisionConfiguration.reset();
}

bool PhysicsManager::Initialize()
{
	//TODO: Make this all customizable

	m_CollisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
	m_CollisionDispatcher = std::make_unique<btCollisionDispatcher>(m_CollisionConfiguration.get());
	m_BroadphaseInterface = std::make_unique<btDbvtBroadphase>();
	m_ConstraintSolver = std::make_unique<btSequentialImpulseConstraintSolver>();

	m_PhysicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_CollisionDispatcher.get(), m_BroadphaseInterface.get(), m_ConstraintSolver.get(), m_CollisionConfiguration.get());

	// Set the gravity for this physics world
	m_PhysicsWorld->setGravity(btVector3(0, -9.8f, 0));

	return true;
}

void PhysicsManager::Update(float aDelta)
{
	m_PhysicsWorld->stepSimulation(aDelta);
}


void PhysicsManager::AddRigidbody(btRigidBody* aRigidbody)
{
	m_PhysicsWorld->addRigidBody(aRigidbody);
	
}

void PhysicsManager::RemoveRigidbody(btRigidBody* aRigidbody)
{
	m_PhysicsWorld->removeRigidBody(aRigidbody);
}