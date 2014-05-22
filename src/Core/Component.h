#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component
{
public:
	int gameObjectID;
	int id;
	Component(void);
	~Component(void);

	virtual void addedToGameObject(GameObject* gameObject);
};

#endif /*COMPONENT_H*/