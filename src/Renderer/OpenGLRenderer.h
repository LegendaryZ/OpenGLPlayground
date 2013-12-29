#pragma once
#include "Renderer.h"

// Include GLEW
#include <gl/glew.h>
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
	GLuint OpenGLRenderer::LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
	virtual BOOL Render();
private:
	HDC hdc; // global device context
	GLuint programID;
	GLuint vertexbuffer;// This will identify our vertex buffer
	BOOL UpdateProjection(void);

};

