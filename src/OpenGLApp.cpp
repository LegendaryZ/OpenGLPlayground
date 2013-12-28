#include "OpenGLApp.h"


void Render()
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//Clear to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear color and depth
    glLoadIdentity(); //Set to initial state

        
    glPushMatrix();
		glLoadIdentity();
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex3f(-10.0, -5  , -30.0);
			glVertex3f(10.0 , -5  , -30.0);
			glVertex3f(0.0  , 10.0, -30.0);
		glEnd();
    glPopMatrix();

    glFlush();

    /*      Bring back buffer to foreground
    */
    SwapBuffers(g_HDC);
}

/**
 * Set the pixel format for the device context
 *   
 * @param HDC	the handle to the main context
 */
void SetupPixelFormat(HDC hDC)
{
    /*      Pixel format index
    */
    int nPixelFormat;

    static PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),          //size of structure
            1,                                      //default version
            PFD_DRAW_TO_WINDOW |                    //window drawing support
            PFD_SUPPORT_OPENGL |                    //opengl support
            PFD_DOUBLEBUFFER,                       //double buffering support
            PFD_TYPE_RGBA,                          //RGBA color mode
            32,                                     //32 bit color mode
            0, 0, 0, 0, 0, 0,                       //ignore color bits
            0,                                      //no alpha buffer
            0,                                      //ignore shift bit
            0,                                      //no accumulation buffer
            0, 0, 0, 0,                             //ignore accumulation bits
            16,                                     //16 bit z-buffer size
            0,                                      //no stencil buffer
            0,                                      //no aux buffer
            PFD_MAIN_PLANE,                         //main drawing plane
            0,                                      //reserved
            0, 0, 0 };                              //layer masks ignored

    /*      Choose best matching format*/
    nPixelFormat = ChoosePixelFormat(hDC, &pfd);

    /*      Set the pixel format to the device context*/
    SetPixelFormat(hDC, nPixelFormat, &pfd);
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
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	 /*      Rendering and Device Context
                variables are declared here.
        */
        static HGLRC hRC;
        static HDC hDC;

        /*      Width and Height for the
                window our robot is to be
                displayed in.
        */
        int width, height;

        switch(message)
        {
                case WM_CREATE: //window being created

                        hDC = GetDC(hwnd);  //get current windows device context
                        g_HDC = hDC;
                        SetupPixelFormat(hDC); //call our pixel format setup function

                        /*      Create rendering context and make it current
                        */
                        hRC = wglCreateContext(hDC);
                        wglMakeCurrent(hDC, hRC);

                        return 0;
                        break;

                case WM_CLOSE:  //window is closing

                        /*      Deselect rendering context and delete it*/
                        wglMakeCurrent(hDC, NULL);
                        wglDeleteContext(hRC);

                        /*      Send quit message to queue*/
                        PostQuitMessage(0);

                        return 0;
                        break;

                case WM_SIZE:

                        /*      Retrieve width and height*/
                        height = HIWORD(lParam);
                        width = LOWORD(lParam);

                        /*      Don't want a divide by 0*/
                        if (height == 0)
                        {
                                height = 1;
                        }

                        /*      Reset the viewport to new dimensions*/
                        glViewport(0, 0, width, height);

                        /*      Set current Matrix to projection*/
                        glMatrixMode(GL_PROJECTION);
                        glLoadIdentity(); //reset projection matrix

                        /*      Time to calculate aspect ratio of
                                our window.
                        */
                        gluPerspective(54.0f, (GLfloat)width/(GLfloat)height, 1.0f, 1000.0f);

                        glMatrixMode(GL_MODELVIEW); //set modelview matrix
                        glLoadIdentity(); //reset modelview matrix

                        return 0;
                        break;

                default:

                        break;
        }

        return (DefWindowProc(hwnd, message, wParam, lParam));;
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
  WNDCLASSEX windowClass; //window class
        HWND    hwnd;                   //window handle
        MSG             msg;                    //message
        bool    done;                   //flag for completion of app
        DWORD   dwExStyle;              //window extended style
        DWORD   dwStyle;                //window style
        RECT    windowRect;

        /*      Screen/display attributes*/
        int width = 800;
        int height = 600;
        int bits = 32;

        windowRect.left =(long)0;               //set left value to 0
        windowRect.right =(long)width;  //set right value to requested width
        windowRect.top =(long)0;                //set top value to 0
        windowRect.bottom =(long)height;//set bottom value to requested height

        /*      Fill out the window class structure*/
        windowClass.cbSize                      = sizeof(WNDCLASSEX);
        windowClass.style                       = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc         = WndProc;
        windowClass.cbClsExtra          = 0;
        windowClass.cbWndExtra          = 0;
        windowClass.hInstance           = hInstance;
        windowClass.hIcon                       = LoadIcon(NULL, IDI_APPLICATION);
        windowClass.hCursor                     = LoadCursor(NULL, IDC_ARROW);
        windowClass.hbrBackground       = NULL;
        windowClass.lpszMenuName        = NULL;
        windowClass.lpszClassName       = L"MyClass";
        windowClass.hIconSm                     = LoadIcon(NULL, IDI_WINLOGO);

        /*      Register window class*/
        if (!RegisterClassEx(&windowClass))
        {
                return 0;
        }

        /*      Check if fullscreen is on*/
        if (fullScreen)
        {
                DEVMODE dmScreenSettings;
                memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
                dmScreenSettings.dmSize = sizeof(dmScreenSettings);
                dmScreenSettings.dmPelsWidth = width;   //screen width
                dmScreenSettings.dmPelsHeight = height; //screen height
                dmScreenSettings.dmBitsPerPel = bits;   //bits per pixel
                dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

                if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN !=
                                                                  DISP_CHANGE_SUCCESSFUL))
                {
                        /*      Setting display mode failed, switch to windowed*/
                        MessageBox(NULL, L"Display mode failed", NULL, MB_OK);
                        fullScreen = false;
                }
        }

        /*      Check if fullscreen is still on*/
        if (fullScreen)
        {
                dwExStyle = WS_EX_APPWINDOW;    //window extended style
                dwStyle = WS_POPUP;                             //windows style
                ShowCursor(FALSE);                              //hide mouse pointer
        }

        else
        {
                dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; //window extended style
                dwStyle = WS_OVERLAPPEDWINDOW;                                  //windows style
        }

        AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

        /*      Class registerd, so now create our window*/
        hwnd = CreateWindowEx(NULL, L"MyClass",  //class name
                                                  L"OpenGL Robot",       //app name
                                                  dwStyle |
                                                  WS_CLIPCHILDREN |
                                                  WS_CLIPSIBLINGS,
                                                  0, 0,                         //x and y coords
                                                  windowRect.right - windowRect.left,
                                                  windowRect.bottom - windowRect.top,//width, height
                                                  NULL,                 //handle to parent
                                                  NULL,                 //handle to menu
                                                  hInstance,    //application instance
                                                  NULL);                //no xtra params

        /*      Check if window creation failed (hwnd = null ?)*/
        if (!hwnd)
        {
                return 0;
        }

        ShowWindow(hwnd, SW_SHOW);      //display window
        UpdateWindow(hwnd);                     //update window

        done = false;   //initialize loop condition variable

        /*      Main message loop*/
        while (!done)
        {
                PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE);

                        if (msg.message == WM_QUIT)     //did we receive a quit message?
                        {
                                done = true;
                        }

                        else
                        {
                                Render();
                                TranslateMessage(&msg);
                                DispatchMessage(&msg);
                        }
        }

        if (fullScreen)
        {
                ChangeDisplaySettings(NULL, 0);
                ShowCursor(TRUE);
        }

        return msg.wParam;
}
