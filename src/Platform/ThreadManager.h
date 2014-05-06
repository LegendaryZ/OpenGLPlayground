#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <thread>
#include <vector>
#include <functional>

using namespace std;

/**
 * Responsible for thread management. Carries out background 
 * tasks and main operations.
 **/
class ThreadManager
{
public:
	static ThreadManager* getInstance();
	~ThreadManager(void);

	void asyncTask(std::function<void (void)> f);
	void joinAll();
private:
	//variables
	vector<std::thread> threads;	

	ThreadManager(void);
};

#endif /*THREADMANAGER_H*/