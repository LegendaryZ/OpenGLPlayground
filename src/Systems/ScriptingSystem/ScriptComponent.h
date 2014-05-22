#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "../../Core/Component.h"

class LuaScript;
class GameObject;

class ScriptComponent : public Component
{
public:
	LuaScript* script;

	ScriptComponent(LuaScript* script);
	~ScriptComponent(void);

	virtual void addedToGameObject(GameObject* gameObject);
};

#endif /*SCRIPTCOMPONENT_H*/