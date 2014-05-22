#include "Director.h"
#include "..\Systems\RenderingSystem\Renderer.h"
#include "..\Platform\Platform.h"
#include "Scene.h"
#include <string>
#include <sstream>

/**
 * Singleton constructor and reference
 *
 * @return director		The singleton Director
 **/
Director* Director::SharedDirector()
{
	static Director* director;
	if(!director)
		director = new Director();

	return director;
}

Director::Director(void)
{
	scene = new Scene();
}


Director::~Director(void)
{
}

/**
 * Pauses the main loop of the program. Should only ever be 
 * called outside of the engine, never internally from within
 * the main loop.
 **/
void Director::Pause()
{
	isActive = false;
	mTimer.Stop();
}

/**
 * Resumes the main loop of the program. Should only ever be 
 * called outside of the engine, never internally from within
 * the main loop.
 **/
void Director::Resume()
{
	isActive = true;
	mTimer.Start();
}

/**
 * Attach to the director to a scene. Only
 * one scene can be attaced at a time.
 *
 * @param scene		The Scene to attach to
 **/
void Director::AttachToScene(Scene* scene)
{
	this->scene = scene;
}

/**
 * @return the current scene or NULL if no scene
 *		   is attached
 **/
Scene* Director::GetCurrentScene()
{
	return scene;
}

/**
 * Attach a renderer to the director
 *
 * @param renderer		the renderer to attach
 **/
void Director::SetRenderingSystem(RenderingSystem* renderer)
{
	this->renderingSystem = renderer;
}

/**
 * @return  the renderer the Director is currently using
 **/
RenderingSystem* Director::GetRenderingSystem()
{
	return renderingSystem;
}

/**
 * Quit out of the app
 **/
void Director::Quit()
{
	isDone = true;
}

/**
 * This is the main loop that controls the updating, rendering,
 * and operating sytem handling of the game.
 *
 * @return result		succesfully ran the game or not
 **/
int Director::Run()
{
	isDone = false;
	int result;
	mTimer.Reset();
	// main loop
	while (!isDone)
	{
		result = HandleOSMessages();//Listen for platform messages

		mTimer.Tick();
		// don't update the scene if the app is minimized
		if (isActive)
		{ //Calculate framerate statistics
			CalculateFrameStats();

			// update the scene every time through the loop
			scene->update(mTimer.DeltaTime());

			// render the scene every time through the loop
			renderingSystem->Render(scene);
		}
	}

	return result;
}

/**
 * Code computes the average frames per second, and also the 
 * average time it takes to render one frame. 
 **/
void Director::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (mTimer.TotalTime() - timeElapsed) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		
		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
		
		std::stringstream ss;
		ss << "Playground	" << "FPS: " << fps;
		SetWindowTitle(ss.str().c_str());
	}
}

#include "GameObject.h"
#include "..\Systems\RenderingSystem\RenderingComponents.h"

Scene* Director::getSampleScene()
{
	Scene* defaultScene = new Scene();
	GameObject* obj = new GameObject(1);
	RenderingComponent* rendComp = new RenderingComponent();
	rendComp->mesh = MeshCache::getInstance()->getMeshData("box");
	//obj.addComponent(new ScriptComponent(LuaScript::create(std::string("src//Systems//ScriptingSystem//Scripts//player.lua"))));
	obj->addComponent(rendComp);
	defaultScene->addGameObject(obj);

	return defaultScene;
}