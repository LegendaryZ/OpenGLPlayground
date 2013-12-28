#pragma once

/*	Trim fat from windows*/
//#define WIN32_LEAN_AND_MEAN	
#pragma comment(linker, "/subsystem:windows")

/*	Pre-processor directives*/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <glut.h>

#define WND_CLASS_NAME  L"OpenGL Window Class"

/*************************** Constants and Macros ***************************/
const int   SCREEN_WIDTH    = 500;
const int   SCREEN_HEIGHT   = 500;
const int   SCREEN_BPP      = 32;
const bool  USE_FULLSCREEN  = false;
LPCWSTR		APP_TITLE      = L"Projections";


/********************************* Globals **********************************/
HDC       g_hdc;                                                                  // global device context
HGLRC     g_hrc;                  // global rendering context
BOOL      g_isFullscreen = TRUE;  // toggles fullscreen and windowed display
BOOL      g_isActive = TRUE;      // false if window is minimized
HWND      g_hwnd = NULL;          // handle of our window
HINSTANCE g_hInstance;            // application instance

BOOL    SetupWindow(LPCWSTR title, int width, int height, int bits, bool isFullscreen);
BOOL    KillWindow();

GLvoid  ResizeScene(GLsizei width, GLsizei height);
BOOL    InitializeScene();
BOOL    DisplayScene();
BOOL    Cleanup();

void    UpdateProjection(GLboolean toggle = GL_TRUE);

void Render();
void SetupPixelFormat(HDC hDC);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


/**
 * Main Function
 *
 * @param hInstance		Is a handle to your applications instance, where an instance 
 *						can be considered to be a single run of your application. The instance 
 *						is used by windows as a reference to your application for event handling,
 *						message processing, and various other duties.
 *
 * @param hPrevInstance is always NULL. 
 *
 * @param lpCmdLine		is a pointer string that is used to hold any command-line
 *						arguments that may have been specified when the application began.  
 *						For example, if the user opened the Run application and typed myapp.exe
 *						myparameter 1, then lpCmdLine would be myparameter 1.
 *
 * @param nShowCMD		is the parameter that determines how your application's window
 *						will be displayed once it begins executing.
 **/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG   msg;       // message
	BOOL  isDone;    // flag indicating when the app is done

	// if the window is set up correctly, we can proceed with the message loop
	if (SetupWindow(APP_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, USE_FULLSCREEN))
	isDone = FALSE;
	// otherwise, we need to never enter the loop and proceed to exit
	else
	isDone = TRUE;

	// main message loop
	while (!isDone)
	{
		if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)   // do we receive a WM_QUIT message?
			{
				isDone = TRUE;              // if so, time to quit the application
			}
			else
			{
				TranslateMessage(&msg);     // translate and dispatch to event queue
				DispatchMessage(&msg);
			}
		}

		// don't update the scene if the app is minimized
		if (g_isActive)
		{
			// update the scene every time through the loop
			DisplayScene();
		}
	}

	Cleanup();
	KillWindow();

	return msg.wParam;
}


