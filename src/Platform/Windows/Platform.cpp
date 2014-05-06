#include "../Platform.h"

#include <string>

#include "../../Systems/RenderingSystem/Renderer.h"
#include "../../Core/Director.h"
#include "WinApp.h"

/**
 * Returns what platform we are currently on
 *
 * @return PlatformType		the platform we are on
 **/
PlatformType DetectPlatform()
{
	return PlatformType::Windows;
}

/**
 * Choose the correct subsytems to spin up
 *
 * @return success or failure
 **/
bool SetUpSubsystems(Director* director)
{
	director->SetRenderingSystem(RenderingSystem::SharedRenderingSystem());

	return true;
}

/**
 * Handle interaction from the os to the process
 *
 * @return the message from the os
 **/
int HandleOSMessages()
{
	int result = 0;
	MSG msg;
	if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		result = msg.wParam;
		if (msg.message == WM_QUIT)   // do we receive a WM_QUIT message?
		{
			Director::SharedDirector()->Quit(); // if so, time to quit the application
		}
		else
		{
			TranslateMessage(&msg);     // translate and dispatch to event queue
			DispatchMessage(&msg);
		}
	}

	return result;
}

/**
 * Set the text in the title bar of the window
 *
 * @param title	The text to be shown
 **/
void SetWindowTitle(const char* title)
{
	SetWindowText(WinApp::getInstance()->getHWND(),WinApp::convertCharToLPCWSTR(title) );
}

/**
 * Swap buffers on the graphics card, displaying the currently rendered scene
 **/
void SwapBuffers()
{
	SwapBuffers(WinApp::getInstance()->getHDC());
}