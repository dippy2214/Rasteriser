#pragma once
#include <map>
#include <Windows.h>

#include "FloatTypes.h"

class InputManager
{
public:
	void Update();
	void OnKeyDown(WPARAM key);
	void OnKeyUp(WPARAM key);
	void OnMouseMove(int x, int y);
	void OnLeftMouseButtonDown();
	void OnLeftMouseButtonUp();

	float2 GetMousePosition();
	float2 GetMouseDragDelta();

	bool IsKeyPressed(int key);
	bool isLeftMouseDown();
	
private:
	std::map<int, bool> keyStates;
	int mouseX = 0;
	int mouseY = 0;
	float2 dragDelta;
	bool leftMouseButtonDown = false;
};

