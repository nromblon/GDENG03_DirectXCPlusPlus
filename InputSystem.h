#pragma once
#include <iostream>
#include "InputListener.h"
#include <Windows.h>
#include <vector>

using namespace std;

class InputSystem
{

public:
	static InputSystem* getInstance();
	static void initialize();
	static void destroy();

	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void update();

	// Global methods for checking key input status
	bool isKeyDown(int key);
	bool isKeyUp(int key);

private:
	static InputSystem* sharedInstance;

	// Keyboard helper methods
	void callOnKeyDown(int key);
	void callOnKeyUp(int key);

	vector<InputListener*> inputListenerList;

	unsigned char keyStates[256] = {};
	unsigned char oldKeyStates[256] = {};

	InputSystem();
	~InputSystem();
	InputSystem(InputSystem const&) {};             // copy constructor is private. You can also do `... = delete`
	InputSystem& operator=(InputSystem const&) {};  // assignment operator is private. You can also do `... = delete`
};

