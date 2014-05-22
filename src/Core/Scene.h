#ifndef SCENE_H
#define SCENE_H

#include <map>

class GameObject;
class Scene
{
public:
	Scene(void);
	~Scene(void);

	bool update(float dt);

	bool addGameObject(GameObject* gameObject);
	bool killGameObjectAndChildren(int id);
	bool killGameObjectDetachChildren(int id);
	bool killGameObjectParentChildren(int id);
	
	inline GameObject* getSceneRoot()
	{ return root; }

protected:
	std::map<int, GameObject*> gameObjects;
	GameObject* root;
};

#endif /*SCENE_H*/