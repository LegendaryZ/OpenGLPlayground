#pragma once

#include <windows.h>

/**
 * Base Level Render. Used to swap Rendering techniques on
 * different platforms
 **/
class Renderer
{
public:
	virtual BOOL Initialize(HDC hdc) = 0;
	virtual BOOL OnResize(int width, int height) = 0;

	virtual BOOL Render() = 0;
	
private:

};