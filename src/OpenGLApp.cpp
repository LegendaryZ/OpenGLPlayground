#include "OpenGLApp.h"

/**
 * Windows Procedure Event Handler
 *
 * @param hwnd		Only important if you have several windows of the same class open
 *					at one time. This is used to determine which window hwnd pointed to before 
 *					deciding on an action.
 *
 * @param message   The actual message identifier that WndProc will be handling.
 *
 * @param wParam	Extension of the message parameter. Used to give 
 *					more information and point to specifics that message cannot convey on its own.
 *
 * @param lParam    Extension of the message parameter. Used to give 
 *					more information and point to specifics that message cannot convey on its own.
 *
 * @return LRESULT  The result of the message being handled. Is a long integer
 **/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	/*	Device Context*/
	HDC hDC; 
	/*	Text for display*/
	LPCWSTR string = L"Hello, World!"; 
	/*	Switch message, condition that is met will execute*/
	switch(message)
	{
		/*	Window is being created*/
		case WM_CREATE: 
			return 0;
			break;
		/*	Window is closing*/
		case WM_CLOSE: 
			PostQuitMessage(0);
			return 0;
			break;
		/*	Window needs update*/
		case WM_PAINT: 
			hDC = BeginPaint(hwnd,&paintStruct);
			/*	Set txt color to blue*/
			SetTextColor(hDC, COLORREF(0x00FF0000));
			/*	Display text in middle of window*/
			TextOut(hDC,150,150,string,13);
			EndPaint(hwnd, &paintStruct);
			return 0;
			break;
		default:
			break;
	}
	return (DefWindowProc(hwnd,message,wParam,lParam));
}


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
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	WNDCLASSEX  windowClass;		//window class
	HWND		hwnd;				//window handle
	MSG			msg;				//message
	bool		done;				//flag saying when app is complete
	/*	Fill out the window class structure*/
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"MyClass";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	/*	Register window class*/
	if (!RegisterClassEx(&windowClass))
	{
		return 0;
	}
	/*	Class registerd, so now create window*/
	hwnd = CreateWindowEx(NULL,		//extended style
		L"MyClass",			//class name
		L"A Real Win App",		//app name
		WS_OVERLAPPEDWINDOW |		//window style
		WS_VISIBLE |
		WS_SYSMENU,
		100,100,			//x/y coords
		400,400,			//width,height
		NULL,				//handle to parent
		NULL,				//handle to menu
		hInstance,			//application instance
		NULL);				//no extra parameter's
	/*	Check if window creation failed*/
	if (!hwnd)
		return 0;
	done = false; //initialize loop condition variable

	/*	main message loop*/
	while(!done)
	{
		PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE);
		if (msg.message == WM_QUIT) //check for a quit message
		{
			done = true; //if found, quit app
		}
		else
		{
			/*	Translate and dispatch to event queue*/
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}