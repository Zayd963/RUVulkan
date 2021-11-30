#include "Window.h"
#include <iostream>
#include <SDL_vulkan.h>
Window::Window(const unsigned int _width, const unsigned int _height, const char* _name)
	:width(_width), height(_height), name(_name)
{
	Init();
}

Window::~Window()
{
	SDL_DestroyWindow(window);
}

void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (SDL_Vulkan_CreateSurface(window, instance, surface) != SDL_TRUE)
		std::cout << "Surface Creation Failed" << std::endl;
}

bool Window::Init()
{
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
	if (!window)
		return false;
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << extensionCount << " Extentions Supported" << std::endl;
	return true;
}
