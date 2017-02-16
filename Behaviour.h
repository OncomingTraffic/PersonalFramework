#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "src\Core\Component.h"

// A behaviour is a type of component that controls how a game object behaves
// Behaviours update once per frame
class Behaviour : public Component
{
public:

	Behaviour();
	~Behaviour();

	virtual bool Initialize();

	// Perform any cleanup steps necessary before this component is removed, such as freeing memory or removing references to it
	virtual void Cleanup();

	virtual void Update(float delta);
};



#endif BEHAVIOUR_H