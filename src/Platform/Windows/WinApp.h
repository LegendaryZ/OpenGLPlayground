#ifndef WINAPP_H
#define WINAPP_H

/*	Pre-processor directives*/
#include <windows.h>
#include "..\..\Core\Director.h"
#include "..\..\Platform\Platform.h"

#define WND_CLASS_NAME  L"WinApp"

class Scene;

/**
 * This is the entry point for the application on
 * the windows platform. This class is responsible for
 * generating a window for the application to run in and
 * handling messages from the window operating system. 
 **/
class WinApp{
public:
	static WinApp* getInstance();
	static LPCWSTR convertCharToLPCWSTR(const char* cstring);

	virtual void setHINSTANCE(HINSTANCE instance);
	virtual HINSTANCE getHINSTANCE();
	virtual HWND getHWND();
	virtual HDC getHDC();

	virtual BOOL    SetupWindow(LPCWSTR title, int width, int height, int bits, bool isFullscreen);
	virtual BOOL    KillWindow();

	virtual BOOL    Initialize();
	virtual BOOL    Cleanup();

	virtual int	Run();

	virtual LRESULT CALLBACK MsgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	Director* director;			  //!< The main director used to facilitate interactions between the OS and the program
	Scene* defaultScene;

	HDC       hdc;                //!< Device context
	HGLRC     hrc;                //!< Rendering context
	BOOL      isFullscreen;		  //!< Toggles fullscreen and windowed display
	HWND      hwnd;		          //!< Handle of our window
	HINSTANCE hInstance;          //!< Application instance

private:

	WinApp();
	~WinApp();

	int     SCREEN_WIDTH;	//!< Width of the window
	int     SCREEN_HEIGHT;	//!< Height of the window
	int     SCREEN_BPP;		//!< Bits per pixel
	bool    USE_FULLSCREEN;	//!< Use full screen or not
	LPCWSTR	APP_TITLE;	    //!< What goes at the top of the window
};

#endif /*WINAPP_H*/








