#include "WinApp.h"

#include "..\..\Systems\RenderingSystem\Renderer.h"

namespace
{
	/**
	 * This is just used to forward Windows messages from a global window
	 * procedure to our member function window procedure because we cannot
	 * assign a member function to WNDCLASS::lpfnWndProc.
	**/
	WinApp* gWinApp = 0;
}
  
/**
 * Forward hwnd on because we can get messages (e.g., WM_CREATE)
 * before CreateWindow returns, and thus before mhMainWnd is valid.
 **/
LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gWinApp->MsgProc(hwnd, msg, wParam, lParam);
}

WinApp* WinApp::getInstance()
{
	static WinApp* app;
	if(!app)
		app = new WinApp();
	return app;
}

/**
 * Basic Constructor that needs a reference to teh HINSTANCE
 * of the app to run
 **/
WinApp::WinApp()
{
	gWinApp = this;
	isFullscreen = FALSE;  // toggles fullscreen and windowed display
	hwnd = NULL;          // handle of our window

	APP_TITLE = L"Projections";

	SCREEN_WIDTH    = 1280;
	SCREEN_HEIGHT   = 720;
	SCREEN_BPP      = 32;
	USE_FULLSCREEN  = false;
}

void WinApp::setHINSTANCE(HINSTANCE instance)
{
	hInstance = instance;
}

HINSTANCE WinApp::getHINSTANCE()
{
	return hInstance;
}

HWND WinApp::getHWND()
{
	return hwnd;
}

HDC WinApp::getHDC()
{
	return hdc;
}

WinApp::~WinApp()
{
}

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
LRESULT CALLBACK WinApp::MsgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_ACTIVATE:  // watch for the window being minimized and restored
		{
			if (!HIWORD(wParam))
			{
				// program was restored or maximized
				director->Resume();
			}
			else
			{
				// program was minimized
				director->Pause();
			}

			return 0;
		}

		case WM_SYSCOMMAND:  // look for screensavers and powersave mode
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:     // screensaver trying to start
				case SC_MONITORPOWER:   // monitor going to powersave mode
					return 0;// returning 0 prevents either from happening
				default:
					break;
			}
		} break;

	case WM_CLOSE:    // window is being closed
	{
		// send WM_QUIT to message queue
		PostQuitMessage(0);
		return 0;
	}

	case WM_SIZE:
	{
		// update perspective with new width and height
		director->GetRenderingSystem()->OnResize(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}

	case WM_CHAR:
	{
		switch (toupper(wParam))
		{
			case VK_ESCAPE:
			{
				// send WM_QUIT to message queue
				PostQuitMessage(0);
				return 0;
			}
			default:
			break;
		};
	} break;

	default:
		break;
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
}

/**
 * Create the window and everything else we need, including the device and
 * rendering context. If a fullscreen window has been requested but can't be
 * created, the user will be prompted to attempt windowed mode. Finally,
 * InitializeScene is called for application-specific setup.
 *
 * @param title			text to put in the title bar
 * @param width			width of the window
 * @param height		height of the window
 * @param bits			bits per pixel
 * @param isFullscreen  whether the app is being run in fullscreen
 * 
 * @return TRUE if everything goes well, or FALSE if an unrecoverable error
 *			occurs. Note that if this is called twice within a program, KillWindow needs
 *			to be called before subsequent calls to SetupWindow.
 **/
BOOL WinApp::SetupWindow(LPCWSTR title, int width, int height, int bits, bool isFullscreen)
{
	// set the global flag
	isFullscreen = isFullscreen;

	// get our instance handle
	hInstance = GetModuleHandle(NULL);

	WNDCLASSEX  wc;    // window class

	// fill out the window class structure
	wc.cbSize         = sizeof(WNDCLASSEX);
	wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc    = MainWndProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = 0;
	wc.hInstance      = hInstance;
	wc.hIcon          = LoadIcon(NULL, IDI_APPLICATION);  // default icon
	wc.hIconSm        = LoadIcon(NULL, IDI_WINLOGO);      // windows logo small icon
	wc.hCursor        = LoadCursor(NULL, IDC_ARROW);      // default arrow
	wc.hbrBackground  = NULL;     // no background needed
	wc.lpszMenuName   = NULL;     // no menu
	wc.lpszClassName  = WND_CLASS_NAME;

	// register the windows class
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Unable to register the window class", L"Error", MB_OK | MB_ICONEXCLAMATION);

		// exit and return FALSE
		return FALSE;
	}

	// if we're in fullscreen mode, set the display up for it
	if (isFullscreen)
	{
		// set up the device mode structure
		DEVMODE screenSettings;
		memset(&screenSettings,0,sizeof(screenSettings));

		screenSettings.dmSize       = sizeof(screenSettings);
		screenSettings.dmPelsWidth  = width;    // screen width
		screenSettings.dmPelsHeight = height;   // screen height
		screenSettings.dmBitsPerPel = bits;     // bits per pixel
		screenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// attempt to switch to the resolution and bit depth we've selected
		if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// if we can't get fullscreen, let them choose to quit or try windowed mode
			if (MessageBox(NULL, L"Cannot run in the fullscreen mode at the selected resolution\n on your video card. Try windowed mode instead?",
                           L"OpenGL Game Programming",
                           MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				isFullscreen = FALSE;
			}
			else
			{
				return FALSE;
			}
		}
	}

	DWORD dwExStyle;
	DWORD dwStyle;

	// set the window style appropriately, depending on whether we're in fullscreen mode
	if (isFullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;           // simple window with no borders or title bar
		ShowCursor(FALSE);            // hide the cursor for now
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	// set up the window we're rendering to so that the top left corner is at (0,0)
	// and the bottom right corner is (height,width)
	RECT  windowRect;
	windowRect.left = 0;
	windowRect.right = (LONG) width;
	windowRect.top = 0;
	windowRect.bottom = (LONG) height;

	// change the size of the rect to account for borders, etc. set by the style
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	// class registered, so now create our window
	hwnd = CreateWindowEx(dwExStyle,          // extended style
	                        WND_CLASS_NAME,     // class name
		                    title,              // app name
		                    dwStyle |           // window style
			                WS_CLIPCHILDREN |   // required for
			                WS_CLIPSIBLINGS,    // using OpenGL
				            0, 0,               // x,y coordinate
			                windowRect.right - windowRect.left, // width
                            windowRect.bottom - windowRect.top, // height
                            NULL,               // handle to parent
                            NULL,               // handle to menu
                            hInstance,        // application instance
                            NULL);              // no extra params

	// see if our window handle is valid
	if (!hwnd)
	{
		MessageBox(NULL, L"Unable to create window", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// get a device context
	if (!(hdc = GetDC(hwnd)))
	{
		MessageBox(NULL, L"Unable to create device context", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// set the pixel format we want
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of structure
		1,                              // default version
		PFD_DRAW_TO_WINDOW |            // window drawing support
		PFD_SUPPORT_OPENGL |            // OpenGL support
		PFD_DOUBLEBUFFER,               // double buffering support
		PFD_TYPE_RGBA,                  // RGBA color mode
		bits,                           // 32 bit color mode
		0, 0, 0, 0, 0, 0,               // ignore color bits, non-palettized mode
		0,                              // no alpha buffer
		0,                              // ignore shift bit
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // ignore accumulation bits
		16,                             // 16 bit z-buffer size
		8,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main drawing plane
		0,                              // reserved
		0, 0, 0 };                      // layer masks ignored

	int  pixelFormat;

	// choose best matching pixel format
	if (!(pixelFormat = ChoosePixelFormat(hdc, &pfd)))
	{
		MessageBox(NULL, L"Can't find an appropriate pixel format", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// set pixel format to device context
	if(!SetPixelFormat(hdc, pixelFormat,&pfd))
	{
		MessageBox(NULL, L"Unable to set pixel format", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// create the OpenGL rendering context
	if (!(hrc = wglCreateContext(hdc)))
	{
		MessageBox(NULL, L"Unable to create OpenGL rendering context", L"Error",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// now make the rendering context the active one
	if(!wglMakeCurrent(hdc, hrc))
	{
		MessageBox(NULL,L"Unable to activate OpenGL rendering context", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// do one-time initialization
	if (!Initialize())
	{
		MessageBox(NULL, L"Initialization failed", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// show the window in the forground, and set the keyboard focus to it
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// set up the perspective for the current screen size
	director->GetRenderingSystem()->OnResize(width, height);

	return TRUE;
} // end SetupWindow()

/**
 * Set up the Director and subsystems and 
 * get them ready to be used by the application
 **/
BOOL WinApp::Initialize()
{
	director = Director::SharedDirector();

	if(!SetUpSubsystems(director))
		return false;

	if(!director->GetRenderingSystem()->Initialize())
		return false;

	return true;
}

/**
 * Deletes the DC, RC, and Window, and restores the original display.
 **/
BOOL WinApp::KillWindow()
{
	// restore the original display if we're in fullscreen mode
	if (isFullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	// if we have an RC, release it
	if (hrc)
	{
		// release the RC
		if (!wglMakeCurrent(NULL,NULL))
		{
			MessageBox(NULL, L"Unable to release rendering context", L"Error", MB_OK | MB_ICONINFORMATION);
		}

		// delete the RC
		if (!wglDeleteContext(hrc))
		{
			MessageBox(NULL, L"Unable to delete rendering context", L"Error", MB_OK | MB_ICONINFORMATION);
		}

		hrc = NULL;
	}

	// release the DC if we have one
	if (hdc && !ReleaseDC(hwnd, hdc))
	{
		MessageBox(NULL, L"Unable to release device context", L"Error", MB_OK | MB_ICONINFORMATION);
		hdc = NULL;
	}

	// destroy the window if we have a valid handle
	if (hwnd && !DestroyWindow(hwnd))
	{
		MessageBox(NULL, L"Unable to destroy window", L"Error", MB_OK | MB_ICONINFORMATION);
		hwnd = NULL;
	}

	// unregister our class so we can create a new one if we need to
	if (!UnregisterClass(WND_CLASS_NAME, hInstance))
	{
		MessageBox(NULL, L"Unable to unregister window class", L"Error", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}

	return TRUE;
} // end KillWindow()

/**
 * Called at the end of successful program execution.
 **/
BOOL WinApp::Cleanup()
{
	return TRUE;
} // end Cleanup()

int WinApp::Run()
{
	int result;
	if (SetupWindow(APP_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, USE_FULLSCREEN))
		result = director->Run();
	
	Cleanup();
	KillWindow();

	return result;
}

LPCWSTR WinApp::convertCharToLPCWSTR(const char* cstring)
{
	const int strLen= strlen(cstring);
	WCHAR* wbuffer = new WCHAR[strLen + 1];
	MultiByteToWideChar( 0,0, cstring, strLen, wbuffer, strLen + 1);
	wbuffer[strLen] = '\0';
	LPCWSTR cstr4 = wbuffer;
	return cstr4;
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
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WinApp* app = WinApp::getInstance();
	app->setHINSTANCE(hInstance);

	return app->Run();
}