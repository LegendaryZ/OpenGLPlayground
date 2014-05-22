#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

Scene::Scene(void)
{
	root = new GameObject(0);
}


Scene::~Scene(void)
{

}

bool Scene::update(float dt)
{

	return true;
}

bool Scene::addGameObject(GameObject* gameObject)
{
	std::map<int, GameObject*>::iterator itr = gameObjects.find(gameObject->getId());
	if(itr == gameObjects.end())
	{
		if(!gameObject->hasParent())
			root->addChild(gameObject);
		gameObjects.insert(std::pair<int, GameObject*>(gameObject->getId(), gameObject));
		
		return true;
	}

	return false;
}