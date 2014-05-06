#ifndef PLATFORM_H
#define PLATFORM_H

class Director;

/**
 * Handle Platform specific tasks and operations
 * needed by the engine
 **/

/**
* The kinds of platforms Playground can run on
*/
enum PlatformType{
	Windows,
	OSX,
	Linux,
	Android,
	Ios
};

PlatformType DetectPlatform();

bool SetUpSubsystems(Director* director);

int HandleOSMessages();

void SetWindowTitle(const char* title);

void SwapBuffers();

#endif /*PLATFORM_H*/