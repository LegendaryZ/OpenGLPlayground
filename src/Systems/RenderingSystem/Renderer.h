#ifndef RENDERER_H
#define RENDERER_H

class Scene;

/**
 * Base Level Render. Used to swap Rendering techniques on
 * different platforms
 **/
class RenderingSystem
{
public:
	static RenderingSystem* SharedRenderingSystem();
		
	~RenderingSystem();

	bool Initialize();
	bool OnResize(int width, int height);
	
	bool Render(Scene* scene);
	
private:
	RenderingSystem();


	//TODO remove for the actual rendering
	unsigned int programID;		
	unsigned int vertexbuffer;		// This will identify our vertex buffer
	unsigned LoadShadersIntoProgram(const char * vertex_file_path,const char * fragment_file_path);

	bool UpdateProjection(void);
};

#endif /*RENDERER_H*/