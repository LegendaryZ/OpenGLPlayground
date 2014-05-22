#include "GameObject.h"
#include "Component.h"
#include "..\Systems\ScriptingSystem\ScriptComponent.h"
#include "..\Systems\RenderingSystem\RenderingComponents.h"
#include <iostream>

GameObject::GameObject(int id)
{
	this->id = id;
	this->parent = NULL;
}

GameObject::~GameObject(void)
{
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

bool GameObject::addComponent(Component* component)
{
	std::map<int, Component*>::iterator itr = components.find(component->id);
	if(itr == components.end())
	{
		components.insert(std::pair<int, Component*>(component->id, component));
		component->addedToGameObject(this);
		return true;
	}

	return false;
}

template <> 
inline ScriptComponent* GameObject::getComponent<ScriptComponent*>(int compId) {
	std::map<int, Component*>::iterator itr = components.find(compId);
	if(itr != components.end())
		return (ScriptComponent*)itr->second;
	else return 0;
}

template <> 
inline RenderingComponent* GameObject::getComponent<RenderingComponent*>(int compId) {
	std::map<int, Component*>::iterator itr = components.find(compId);
	if(itr != components.end())
		return (RenderingComponent*)itr->second;
	else return 0;
}

bool GameObject::addChild(GameObject* child)
{
	child->setParent(this);
	children.push_back(child);

	return true;
}