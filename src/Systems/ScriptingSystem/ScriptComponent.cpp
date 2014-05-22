#include "ScriptComponent.h"
#include "LuaScript.h"
#include <string>
#include <iostream>

ScriptComponent::ScriptComponent(LuaScript* script) : Component()
{
	this->script = script;
	id = script->get<int>(std::string("id"));
}


ScriptComponent::~ScriptComponent(void)
{
}

void ScriptComponent::addedToGameObject(GameObject* gameObject)
{
	Component::addedToGameObject(gameObject);
	script->set<int>(std::string("gameObjectId"), gameObjectID);
}