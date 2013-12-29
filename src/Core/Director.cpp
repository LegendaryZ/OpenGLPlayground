#include "Director.h"
#include "../Renderer/Renderer.h"
#include "../Platform/Platform.h"

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

void Director::setActive(bool active)
{
	isActive = active;
}

void Director::setRenderer(Renderer* renderer)
{
	this->renderer = renderer;
}

Renderer* Director::getRenderer()
{
	return renderer;
}

void Director::update()
{
}

void Director::quit()
{
	isDone = true;
}

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