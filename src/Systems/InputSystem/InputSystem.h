#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

class InputSystem
{
public:
	static InputSystem* sharedInputSystem();
	~InputSystem();

	void startListening();

private:
	InputSystem();
}

#endif /*INPUTSYSTEM_H*/