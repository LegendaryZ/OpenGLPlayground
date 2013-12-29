#include "Director.h"
#include "../Renderer/Renderer.h"
#include "../Platform/Platform.h"

/**
 * Singleton constructor and reference
 *
 * @return director		The singleton Director
 **/
Director* Director::getSharedDirector()
{
	static Director* director;
	if(!director)
		director = new Director();

	return director;
}

Director::Director(void)
{
}


Director::~Director(void)
{
}

/**
 * Tell the director whether the window is active (not minimized)
 * or not
 *
 * @param active	is the window minimized (false) or not (true)
 **/
void Director::setActive(bool active)
{
	isActive = active;
}

/**
 * Attach a renderer to the director
 *
 * @param renderer		the renderer to attach
 **/
void Director::setRenderer(Renderer* renderer)
{
	this->renderer = renderer;
}

/**
 * @return  the renderer the Director is currently using
 **/
Renderer* Director::getRenderer()
{
	return renderer;
}

/**
 * Update the current scene
 **/
void Director::update()
{
}

/**
 * Quit out of the app
 **/
void Director::quit()
{
	isDone = true;
}

/**
 * This is the main loop that controls the updating, rendering,
 * and operating sytem handling of the game.
 *
 * @return result		succesfully ran the game or not
 **/
int Director::run()
{
	isDone = false;
	int result;
	// main loop
	while (!isDone)
	{
		result = Platform::getInstance()->PlatformProcess();//Listen for platform messages

		// don't update the scene if the app is minimized
		if (isActive)
		{
			// update the scene every time through the loop
			update();

			// render the scene every time through the loop
			renderer->Render();
		}
	}

	return result;
}