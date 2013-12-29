#pragma once
#include "Renderer.h"
#include <gl/GL.h>
#include <gl/GLU.h>

/**
 * OpenGL Based Renderer. Should be able to be used cross platform
 **/
class OpenGLRenderer : public Renderer
{
public:
	OpenGLRenderer(void);
	~OpenGLRenderer(void);

	virtual BOOL Initialize(HDC hdc);
	virtual BOOL OnResize(int width, int height);

	virtual BOOL Render();
private:
	HDC hdc; // global device context

	BOOL UpdateProjection(void);

};

