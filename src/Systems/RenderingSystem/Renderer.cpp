#include "Renderer.h"
#include "..\..\Core\Scene.h"
#include "..\..\Platform\Platform.h"
#include "..\..\Core\GameObject.h"
#include "..\..\..\gl\include\glew.h"
#include "..\..\..\gl\include\glut.h"
#include "RenderingComponents.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <random>
#include <cstdlib>

RenderingSystem* RenderingSystem::SharedRenderingSystem()
{
	static RenderingSystem* renderingSystem;
	if(!renderingSystem)
		renderingSystem = new RenderingSystem();
	return renderingSystem;
}

RenderingSystem::RenderingSystem(void)
{
}


RenderingSystem::~RenderingSystem(void)
{
}

/**
 * Performs one-time application-specific setup.
 *
 * @return FALSE on any failure. TRUE if success
 **/
bool RenderingSystem::Initialize()
{
	if (glewInit() != GLEW_OK)
		return false;

	// some GL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

	programID = LoadShadersIntoProgram( "src\\Systems\\RenderingSystem\\Shaders\\SimpleVertexShader.vertexshader", "src\\Systems\\RenderingSystem\\Shaders\\SimpleFragmentShader.fragmentshader" );


	return true;
}

/**
 * Loads a vertex shader and a fragment shader from two different files 
 * and places them into a program that can be used by opengl
 *
 * @param vertex_file_path		The path to the vertex shader
 * @param fragment_file_path	The path to the fragement shader
 *
 * @return ProgramID			The id of the program using the shaders within opengl
 **/
GLuint RenderingSystem::LoadShadersIntoProgram(const char * vertex_file_path,const char * fragment_file_path)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

/**
 * Called once when the application starts and again every time the window is
 * resized by the user.
 *
 * @param width		the new width of the window
 * @param height	the new height of the window
 **/
bool RenderingSystem::OnResize(int width, int height)
{
	// avoid divide by zero
	if (height == 0)
		height = 1;

	// reset the viewport to the new dimensions
	glViewport(0, 0, width, height);

	// set up the projection, without toggling the projection mode
	UpdateProjection();

	return true;
}

/**
 * Sets the current projection mode to a perspective
 * projection
 **/
bool RenderingSystem::UpdateProjection(void)
{
	// select the projection matrix and clear it out
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// set the perspective with the appropriate aspect ratio
	glFrustum(-1.0, 1.0, -0.5625, 0.5625, 5, 100);

	// select modelview matrix and clear it out
	glMatrixMode(GL_MODELVIEW);

	return true;
}

/**
 * Renders the current scene
 *
 * @return success or failure
 **/
bool RenderingSystem::Render(Scene* scene)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Use the shader
	glUseProgram(programID);

	GameObject* go = scene->getSceneRoot();
	for(int i = 0; i < go->getNumChildren();i++)
	{
		Render(go->getChildren()[i]);
	}
	// switch the front and back buffers to display the updated scene
	SwapBuffers();
	return true;
}

bool RenderingSystem::Render(GameObject* go)
{
	if(go->hasComponent(11))
		go->getComponent<RenderingComponent*>(11)->draw();
	for(int i = 0; i < go->getNumChildren();i++)
	{
		Render(go->getChildren()[i]);
	}
	return true;
}