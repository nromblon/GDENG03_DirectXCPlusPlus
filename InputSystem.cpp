#include "InputSystem.h"

InputSystem* InputSystem::sharedInstance = nullptr;

InputSystem* InputSystem::getInstance()
{
	return sharedInstance;
}

void InputSystem::initialize()
{
	sharedInstance = new InputSystem();
}

void InputSystem::destroy()
{
	delete sharedInstance;
}

void InputSystem::addListener(InputListener* listener)
{
	this->inputListenerList.push_back(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
	//find object in used
	int index = -1;
	for (int i = 0; i < this->inputListenerList.size(); i++) {
		if (this->inputListenerList[i] == listener) {
			index = i;
			break;
		}
	}
	if (index > -1) {
		this->inputListenerList.erase(this->inputListenerList.begin() + index);
	}
}

void InputSystem::update()
{
	if (GetKeyboardState(keyStates)) //update keyStates
	{
		for (int i = 0; i < ARRAYSIZE(keyStates); i++)
		{
			// this->keyStates[i] & 0x80 checks whether a key has been pressed (regardless of whether it is toggled / untoggled)
			if (this->keyStates[i] & 0x80 && this->keyStates[i] != this->oldKeyStates[i])
			{
				callOnKeyDown(i);
			}
			else if (this->keyStates[i] != this->oldKeyStates[i])
			{
				callOnKeyUp(i);
			}
		}

		//store current keys to old states
		::memcpy(this->oldKeyStates, this->keyStates, sizeof(unsigned char) * 256);
	}
}

bool InputSystem::isKeyDown(int key)
{
	for (int i = 0; i < ARRAYSIZE(this->keyStates); i++) {
		if (this->keyStates[i] & 0x80 && i == key) {
			return true;
		}
		else if (i == key) {
			return false;
		}
	}


	return false;
}

bool InputSystem::isKeyUp(int key)
{
	for (int i = 0; i < ARRAYSIZE(this->keyStates); i++) {
		if (!(this->keyStates[i] & 0x80) && i == key) {
			return true;
		}
		else if (i == key) {
			return false;
		}
	}

	return false;
}

void InputSystem::callOnKeyDown(int key)
{
	for (int i = 0; i < this->inputListenerList.size(); i++) {
		this->inputListenerList[i]->onKeyDown(key);
	}
}

void InputSystem::callOnKeyUp(int key)
{
	for (int i = 0; i < this->inputListenerList.size(); i++) {
		this->inputListenerList[i]->onKeyUp(key);
	}
}

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}


