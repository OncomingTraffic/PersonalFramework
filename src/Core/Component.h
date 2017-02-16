#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <string>


class BaseObject;


/*
-Abstract-
Represents a component that can be added to a GameObject, but is not a GameObject itself
Components should never be instantiated on their own, only attached to existing GameObjects
*/
class Component
{
public:

	Component();
	virtual ~Component();

	// Initialize this component. Returns false if anything failed to initialize
	// The component should not be attached to a GameObject if initialization fails
	//TODO: should Initialize() return anything? Otherwise it would just throw errors. Which components could fail initialization?
	virtual bool Initialize() = 0;

	// Perform any cleanup steps necessary before this component is removed, such as freeing memory or removing references to it
	virtual void Cleanup();

	void AttachToGameObject(BaseObject* object);

	BaseObject* GetParent();

	// Get the class type of this object as a string
	virtual std::string GetType() = 0;


protected:

	// The GameObject that this component is attached to
	BaseObject* m_Parent;
};


#endif //COMPONENT_H