#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "btBulletDynamicsCommon.h"
#include <memory>
#include <vector>


class RigidBody;

class PhysicsManager
{
public:

	PhysicsManager();
	~PhysicsManager();

	bool Initialize();

	void Update(float delta);

	void AddRigidbody(btRigidBody* rigidbody);

	void RemoveRigidbody(btRigidBody* rigidbody);

private:

	// TODO: Change this to a PhysicsWorld base class if necessary
	std::unique_ptr<btDynamicsWorld> m_PhysicsWorld;

	// PhysicsWorld uses these to calculate collisions
	std::unique_ptr<btDefaultCollisionConfiguration> m_CollisionConfiguration;
	std::unique_ptr<btCollisionDispatcher> m_CollisionDispatcher;
	std::unique_ptr<btBroadphaseInterface> m_BroadphaseInterface;
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_ConstraintSolver;

	// A list of all the collision shapes, so that they can be reused, and properly deleted
	std::vector<std::unique_ptr<btCollisionShape>> m_CollisionShapes;
};


#endif //PHYSICSMANAGER_H