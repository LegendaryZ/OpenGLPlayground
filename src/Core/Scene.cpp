#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

Scene::Scene(void)
{
}


Scene::~Scene(void)
{

}

bool Scene::update(float dt)
{
	return true;
}

bool Scene::addGameObject(GameObject gameObject)
{
	gameObjects.push_back(gameObject);
	return false;
}