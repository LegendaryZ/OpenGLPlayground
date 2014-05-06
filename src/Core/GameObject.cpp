#include "GameObject.h"
#include "Component.h"
#include "..\Systems\ScriptingSystem\ScriptComponent.h"
#include <iostream>

GameObject::GameObject(int id)
{
	this->id = id;
}

GameObject::~GameObject(void)
{
	std::map<ComponentType, Component*>::iterator itr = components.begin();
	for(itr; itr != components.end();itr++)
		delete itr->second;
	components.clear();
}

int count = 0;
bool GameObject::update(float dt)
{
	printf("\nupdate gameobject\n");
	count++;
	if(count >= 5)
		return true;
	return false;
}

bool GameObject::initialize()
{
	return true;
}

bool GameObject::addComponent(ComponentType type, Component* component, bool replaceExisting)
{
	std::map<ComponentType, Component*>::iterator itr = components.find(type);
	if(itr == components.end())
	{
		components.insert(std::pair<ComponentType, Component*>(type, component));
		component->addedToGameObject(this);
		return true;
	}
	else if(replaceExisting)
	{
		delete itr->second;
		components.erase(itr);
		components.insert(std::pair<ComponentType, Component*>(type, component));
		component->addedToGameObject(this);
		return true;
	}
	return false;
}

template <> 
inline ScriptComponent* GameObject::getComponent<ScriptComponent*>(ComponentType type) {
	std::map<ComponentType, Component*>::iterator itr = components.find(type);
	if(itr != components.end())
		return (ScriptComponent*)itr->second;
	else return 0;
}

