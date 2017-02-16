#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "../src/Core/Component.h"
#include "glm\glm.hpp"


class PhysicsManager;

class RigidBody : public Component
{
public:

	RigidBody();
	RigidBody(PhysicsManager* physicsManager);
	~RigidBody();

	bool Initialize();

	void Update();

	void UpdateParentPosition();
	void UpdateRigidBodyPosition();

	std::unique_ptr<btRigidBody>& GetRigidBody();

	void SetMass(float mass);
	float GetMass();

	void SetCollisionShape(std::shared_ptr<btCollisionShape> collisionShape);
	std::shared_ptr<btCollisionShape> GetCollisionShape();

	void SetPhysicsManager(PhysicsManager* physicsManager);
	PhysicsManager* GetPhysicsManager();

	std::string GetType();

private:

	std::unique_ptr<btRigidBody> m_RigidBody;

	float m_Mass;

	std::shared_ptr<btCollisionShape> m_CollisionShape;

	std::unique_ptr<btDefaultMotionState> m_MotionState;

	bool m_IsLoaded;

	PhysicsManager* m_PhysicsManager;
};

glm::vec3 ToGlmVec3(const btVector3& bulletVector);

btVector3 ToBulletVec3(const glm::vec3& glmVector);

#endif //RIGIDBODY_H