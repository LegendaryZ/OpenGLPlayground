#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <map>

class Component;

class GameObject
{
public:
	GameObject(int id);
	~GameObject(void);

	bool initialize();
	bool update(float dt);

	bool addComponent(Component* component);

	template<typename T>
	T getComponent(int compId)
	{ return 0;}

	bool hasComponent(int compId)
	{ return components.find(compId) != components.end(); }

	int getId()
	{ return id; }

	void setParent(GameObject* parent)
	{ this->parent = parent;}

	GameObject* getParent()
	{ return parent; }

	bool hasParent()
	{ return parent != NULL; }

	bool addChild(GameObject* child);

	std::vector<GameObject*>& getChildren()
	{ return children;}

	int getNumChildren()
	{ return children.size(); }

protected:
	int id;
	std::map<int, Component*> components;

	std::vector<GameObject*> children;
	GameObject* parent; //<! Weak reference to parent
};

#endif /*GAMEOBJECT_H*/