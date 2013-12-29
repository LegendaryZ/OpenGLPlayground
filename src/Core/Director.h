#pragma once

class Renderer;

/**
 * The controller of the main loop. Also
 * holds the different subsystems. 
 **/
class Director
{
public:
	static Director* getSharedDirector();

	void setActive(bool active);

	void setRenderer(Renderer* renderer);
	Renderer* getRenderer();

	int run();
	void quit();
private:
	Renderer* renderer;
	bool isActive;
	bool isDone;

	Director(void);
	~Director(void);
	void update();
};

