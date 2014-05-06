#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <map>

class Component;

class GameObject
{
public:
	enum ComponentType{
		LuaScript
	};

	GameObject(int id);
	~GameObject(void);

	bool initialize();
	bool update(float dt);

	bool addComponent(ComponentType type, Component* component, bool replaceExisting = false);

	template<typename T>
	T getComponent(ComponentType type)
	{ return 0;}

	int getId()
	{ return id; }

protected:
	int id;
	std::map<ComponentType, Component*> components;

	std::vector<GameObject*> children;
	GameObject* parent;
};

#endif /*GAMEOBJECT_H*/