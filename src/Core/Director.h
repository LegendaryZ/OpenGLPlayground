#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "..\Platform\FrameTimer.h"

class RenderingSystem;
class Scene;


/**
 * The controller of the main loop. Also
 * holds the different subsystems. 
 **/
class Director
{
public:
	static Director* SharedDirector();
	~Director(void);

	void Pause();
	void Resume();
	int Run();
	void Quit();

	void AttachToScene(Scene* scene);
	Scene* GetCurrentScene();

	void SetRenderingSystem(RenderingSystem* RenderingSystem);
	RenderingSystem* GetRenderingSystem();

private:
	FrameTimer mTimer;	//<! Used to calculate the frame rate and Delta Time between frames
	Scene* scene;		//<! The current model for the application
	RenderingSystem* renderingSystem; //<! The RenderingSystem used to display the model
	bool isActive;		//<! Whether the the application is currently being interacted with
	bool isDone;		//<! Has the application finished running

	Director(void);


	void CalculateFrameStats();
};

#endif /*DIRECTOR_H*/
