#pragma once

class Renderer;

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

