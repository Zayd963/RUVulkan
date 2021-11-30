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
	void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
private:

	bool Init();

	const unsigned int width;
	const unsigned int height;
	const char* name;

	SDL_Window* window = nullptr;
};

