#pragma once
#include <SDL.h>
#include <vulkan.h>
class Window
{
public:
	Window(const unsigned int _width, const unsigned int _height, const char* name);
	~Window();
	SDL_Window* GetWindow() { return window; }
	VkExtent2D GetExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
	bool WasWindowRezied() { return frameBufferResized; }
	void ResetWindowResizeFlag() { frameBufferResized = false; }
	void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	int width;
	int height;
	const char* name;

	bool frameBufferResized;
private:

	bool Init();



	



	SDL_Window* window = nullptr;
};

