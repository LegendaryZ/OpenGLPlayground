#pragma once

/*	Trim fat from windows*/
#define WIN32_LEAN_AND_MEAN	
#pragma comment(linker, "/subsystem:windows")

/*	Pre-processor directives*/
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);