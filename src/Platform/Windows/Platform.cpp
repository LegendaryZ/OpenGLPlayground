#include "../Platform.h"

#include "../../Renderer/Renderer.h"
#include "../../Renderer/OpenGLRenderer.h"
#include "../../Core/Director.h"

/**
 * The singleton constructor/referencer for the Platform
 *
 * @return Platform		the current platform
 **/
Platform* Platform::getInstance()
{
	static Platform* platform;
	if(!platform)
		platform = new Platform();

	return platform;
}
	
Platform::Platform(void)
{
	platformType = DetectPlatform();
}

Platform::~Platform()
{
}

/**
 * Returns what platform we are currently on
 *
 * @return PlatformType		the platform we are on
 **/
Platform::PlatformType Platform::DetectPlatform()
{
	return Platform::PlatformType::Windows;
}

/**
 * Choose the correct subsytems to spin up
 *
 * @return success or failure
 **/
bool Platform::SetUpSubsystems(Director* director)
{
	director->setRenderer(new OpenGLRenderer());

	return true;
}

/**
 * Handle interaction from the os to the process
 *
 * @return the message from the os
 **/
int Platform::PlatformProcess()
{
	int result = 0;
	MSG msg;
	if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		result = msg.wParam;
		if (msg.message == WM_QUIT)   // do we receive a WM_QUIT message?
		{
			Director::getSharedDirector()->quit(); // if so, time to quit the application
		}
		else
		{
			TranslateMessage(&msg);     // translate and dispatch to event queue
			DispatchMessage(&msg);
		}
	}
	return result;
}