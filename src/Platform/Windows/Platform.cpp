#include "../Platform.h"

#include "../../Renderer/Renderer.h"
#include "../../Renderer/OpenGLRenderer.h"
#include "../../Core/Director.h"

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

Platform::PlatformType Platform::DetectPlatform()
{
	return Platform::PlatformType::Windows;
}

bool Platform::SetUpSubsystems(Director* director)
{
	director->setRenderer(new OpenGLRenderer());

	return true;
}

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