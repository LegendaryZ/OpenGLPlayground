#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "../../Core/Component.h"

class LuaScript;
class GameObject;

class ScriptComponent : public Component
{
public:
	ScriptComponent(LuaScript* script);
	~ScriptComponent(void);

	virtual void addedToGameObject(GameObject* gameObject);
private:
	LuaScript* script;
};

#endif /*SCRIPTCOMPONENT_H*/