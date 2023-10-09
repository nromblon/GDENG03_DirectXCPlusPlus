#pragma once
// Interface for Input Listeners

class InputListener {

public:
	virtual void onKeyUp(int key) = 0;
	virtual void onKeyDown(int key) = 0;
};
