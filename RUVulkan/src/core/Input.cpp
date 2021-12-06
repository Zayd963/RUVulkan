#include "Input.h"
#include <algorithm>
#include <iostream>
Uint8 Input::lastFrameKeys[] = { 0 };
Uint8 Input::currentFrameKeys[] = { 0 };
Uint32 Input::currentMouseButtons = 0;
Uint32 Input::lastMouseButton = 0;
int Input::mouseX = 0;
int Input::mouseY = 0;
bool Input::run = true;
void Input::Listen()
{

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			run = false;
		}


	}
	memcpy(lastFrameKeys, currentFrameKeys, sizeof(Uint8) * SDL_NUM_SCANCODES);
	memcpy(currentFrameKeys, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
	lastMouseButton = currentMouseButtons;
	currentMouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
}

bool Input::isKeyDown(SDL_Scancode scancode)
{
	if (lastFrameKeys[scancode] == false && currentFrameKeys[scancode] == true)
		return true;
	return false;
}

bool Input::isKeyHeld(SDL_Scancode scancode)
{
	if (lastFrameKeys[scancode] == true && currentFrameKeys[scancode] == true)
		return true;
	return false;
}

bool Input::isKeyUp(SDL_Scancode scancode)
{
	if (lastFrameKeys[scancode] == true && currentFrameKeys[scancode] == false)
		return true;
	return false;
}

bool Input::isKey(SDL_Scancode scancode)
{
	if (lastFrameKeys[scancode] == false && currentFrameKeys[scancode] == true || lastFrameKeys[scancode] == true && currentFrameKeys[scancode] == true)
		return true;
}

int Input::GetMouseX()
{
	return mouseX;
}

int Input::GetMouseY()
{
	return mouseY;
}


bool Input::isMouseDown(Uint32 button)
{
	return (!(lastMouseButton & button) && (currentMouseButtons & button));
}

bool Input::isMouseHeld(Uint32 button)
{
	return ((lastMouseButton & button) && (currentMouseButtons & button));
}

bool Input::isMouseUp(Uint32 button)
{
	return ((lastMouseButton & button) && ((currentMouseButtons & button) == 0));
}

bool Input::isMouse(Uint32 button)
{
	return (!(lastMouseButton & button) && (currentMouseButtons & button)) ||
		((lastMouseButton & button) && (currentMouseButtons & button));
}

