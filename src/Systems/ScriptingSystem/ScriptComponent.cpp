#include "ScriptComponent.h"
#include "LuaScript.h"
#include <string>
#include <iostream>

ScriptComponent::ScriptComponent(LuaScript* script) : Component()
{
	this->script = script;
}


ScriptComponent::~ScriptComponent(void)
{
}

void ScriptComponent::addedToGameObject(GameObject* gameObject)
{
	Component::addedToGameObject(gameObject);

	script->set<int>(std::string("id"), gameObjectID);

	printf("GameObject id: %d", script->get<int>("id"));
}