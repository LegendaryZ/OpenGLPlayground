#ifndef SCENE_H
#define SCENE_H

#include <vector>

class GameObject;
class Scene
{
public:
	Scene(void);
	~Scene(void);

	virtual bool update(float dt);

	virtual bool addGameObject(GameObject gameObject);

protected:
	std::vector<GameObject> gameObjects;
};

#endif /*SCENE_H*/