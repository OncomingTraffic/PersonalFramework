#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include "src\Core\Component.h"
#include "Behaviour.h"


class PhysicsObject;


class PlayerControl : public Behaviour
{
public:

	PlayerControl();
	virtual ~PlayerControl();

	// Initialize this component. Returns false if anything failed to initialize
	// The component should not be attached to a GameObject if initialization fails
	virtual bool Initialize();

	// Perform any cleanup steps necessary before this component is removed, such as freeing memory or removing references to it
	virtual void Cleanup();

	virtual void Update(float delta);

	// Get the class type of this object as a string
	virtual std::string GetType();


protected:

	PhysicsObject* m_PhysicsObject;
};




#endif //PLAYERCONTROL_H