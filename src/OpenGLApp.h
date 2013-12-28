#pragma once

/*	Trim fat from windows*/
//#define WIN32_LEAN_AND_MEAN	
#pragma comment(linker, "/subsystem:windows")

/*	Pre-processor directives*/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

//Bad Global Variabls, Bad. TODO remove and put into a better system
HDC g_HDC;
bool fullScreen = false;

void Render();
void SetupPixelFormat(HDC hDC);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
