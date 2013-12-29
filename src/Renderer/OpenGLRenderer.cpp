#include "OpenGLRenderer.h"
#include "../../gl/glut.h"

OpenGLRenderer::OpenGLRenderer(void)
{
}


OpenGLRenderer::~OpenGLRenderer(void)
{
}

/**
 * Performs one-time application-specific setup. Returns FALSE on any failure.
 **/
BOOL OpenGLRenderer::Initialize(HDC hdc)
{
	this->hdc = hdc;
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	return TRUE;
}

/**
 * Called once when the application starts and again every time the window is
 * resized by the user.
 *
 * @param width		the new width of the window
 * @param height	the new height of the window
 **/
BOOL OpenGLRenderer::OnResize(int width, int height)
{
	// avoid divide by zero
	if (height==0)
	{
		height=1;
	}

	// reset the viewport to the new dimensions
	glViewport(0, 0, width, height);

	// set up the projection, without toggling the projection mode
	UpdateProjection();

	return TRUE;
}

/**
 * Sets the current projection mode to a perspective
 * projection
 **/
BOOL OpenGLRenderer::UpdateProjection(void)
{
	// select the projection matrix and clear it out
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// set the perspective with the appropriate aspect ratio
	glFrustum(-1.0, 1.0, -0.5625, 0.5625, 5, 100);

	// select modelview matrix and clear it out
	glMatrixMode(GL_MODELVIEW);

	return TRUE;
}

BOOL OpenGLRenderer::Render()
{
	GLfloat yellow[4] = { 1.0f, 1.0f, 0.2f, 1.0f };
	GLfloat blue[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
	GLfloat green[4] = { 0.2f, 1.0f, 0.2f, 1.0f };

	glLoadIdentity();
	gluLookAt(-0.5, 1.0, 7.0,
		       0.0, 0.0, 0.0,
			   0.0, 1.0, 0.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
	glPushMatrix();
		glTranslatef(0.3, 0.0, 1.0);
		glutSolidCube(0.5);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
	glPushMatrix();
		glutSolidCube(0.5);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
	glPushMatrix();
		glTranslatef(-0.3, 0.0, -1.0);
		glutSolidCube(0.5);
	glPopMatrix();

	// switch the front and back buffers to display the updated scene
	SwapBuffers(hdc);
  
	return TRUE;
}
