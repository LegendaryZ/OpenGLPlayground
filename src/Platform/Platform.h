#pragma once

class Director;

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

