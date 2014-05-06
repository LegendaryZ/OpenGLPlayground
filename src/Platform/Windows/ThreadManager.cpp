#include "../ThreadManager.h"

/**
 * Singleton accessor/constructor
 **/
ThreadManager* ThreadManager::getInstance()
{
	static ThreadManager* threadManager;
	if(!threadManager)
		threadManager = new ThreadManager();
	return threadManager;
}

ThreadManager::ThreadManager(void)
{}

ThreadManager::~ThreadManager(void)
{}

/**
 * Launches a task to be carried out in the background
 *
 * @param f		the lambda function to be carried out
 **/
void ThreadManager::asyncTask(std::function<void (void)> f)
{
	threads.push_back(thread(f));
}

void ThreadManager::joinAll()
{
	for(int i = 0; i < threads.size();i++)
	{
		if(threads[i].joinable())
			threads[i].join();
	}
}
