#pragma once

class Director;

/**
 * Set up the engine for the different supported platforms.
 * Will detect the current platform and spin up the differnt
 * subsystems correctly. 
 **/
class Platform
{
public:
	enum PlatformType{
		Windows,
		OSX,
		Linux
	};

	static Platform* getInstance();
	
	PlatformType DetectPlatform();

	bool SetUpSubsystems(Director* director);

	int PlatformProcess();

private:
	PlatformType platformType;

	Platform(void);
	~Platform(void);
};

