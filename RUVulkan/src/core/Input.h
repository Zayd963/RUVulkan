#pragma once
#include <SDL.h>
class Input
{
public:

	static void Listen();
	static bool isKeyDown(SDL_Scancode scancode);
	static bool isKeyHeld(SDL_Scancode scancode);
	static bool isKeyUp(SDL_Scancode scancode);
	static bool isKey(SDL_Scancode scancode);
	static int GetMouseX();
	static int GetMouseY();

	static bool isMouseDown(Uint32 button);
	static bool isMouseHeld(Uint32 button);
	static bool isMouseUp(Uint32 button);
	static bool isMouse(Uint32 button);

	static bool isRunning() { return run; }

private:
	static Uint8 lastFrameKeys[SDL_NUM_SCANCODES];
	static Uint8 currentFrameKeys[SDL_NUM_SCANCODES];
	static Uint32 currentMouseButtons;
	static Uint32 lastMouseButton;
	static int mouseX;
	static int mouseY;
	static bool run;

};

