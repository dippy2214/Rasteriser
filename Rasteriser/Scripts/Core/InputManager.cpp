#include "InputManager.h"

bool InputManager::IsKeyPressed(int key)
{
    return keyStates[key];
}

bool InputManager::isLeftMouseDown()
{
    return leftMouseButtonDown;
}

float2 InputManager::GetMousePosition()
{
    return float2(mouseX, mouseY);
}

void InputManager::Update()
{
    dragDelta = float2(0, 0);
}

void InputManager::OnKeyDown(WPARAM key)
{
    keyStates[key] = true;
}

void InputManager::OnKeyUp(WPARAM key)
{
    keyStates[key] = false;
}

void InputManager::OnMouseMove(int x, int y)
{
    dragDelta = float2(mouseX - x, mouseY - y);
    mouseX = x;
    mouseY = y;
    
}

float2 InputManager::GetMouseDragDelta()
{
    return dragDelta;
}

void InputManager::OnLeftMouseButtonUp()
{
    leftMouseButtonDown = false;
    ReleaseCapture();
}

void InputManager::OnLeftMouseButtonDown()
{
    leftMouseButtonDown = true;
    SetCapture(GetForegroundWindow());
}
