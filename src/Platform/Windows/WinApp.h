#pragma once


#pragma comment(linker, "/subsystem:windows")

/*	Pre-processor directives*/
#include <windows.h>
#include "..\..\Core\Director.h"
#include "..\..\Platform\Platform.h"

#define WND_CLASS_NAME  L"OpenGL Window Class"

class WinApp{
public:
	WinApp(HINSTANCE hInstance);
	~WinApp();

	/******************************* Main Methods *******************************/
	virtual BOOL    SetupWindow(LPCWSTR title, int width, int height, int bits, bool isFullscreen);
	virtual BOOL    KillWindow();

	virtual BOOL    Initialize();

	virtual BOOL    Cleanup();

	virtual int	Run();

	virtual LRESULT CALLBACK MsgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	Director* director;
	Platform* platform;

	/********************************* Globals **********************************/
	HDC       hdc;                // global device context
	HGLRC     hrc;                // global rendering context
	BOOL      isFullscreen;		  // toggles fullscreen and windowed display
	BOOL      isActive;		      // false if window is minimized
	HWND      hwnd;		          // handle of our window
	HINSTANCE hInstance;          // application instance

private:
	/*************************** Constants and Macros ***************************/
	int   SCREEN_WIDTH;
	int   SCREEN_HEIGHT;
	int   SCREEN_BPP;
	bool  USE_FULLSCREEN;
	LPCWSTR		APP_TITLE;
};










