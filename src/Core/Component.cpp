#include "Component.h"
#include "GameObject.h"

Component::Component(void)
{
	gameObjectID = 0;
}


Component::~Component(void)
{
}

void Component::addedToGameObject(GameObject* gameObject)
{
	gameObjectID = gameObject->getId();
}
