#pragma once
#include "Window/Window.h"
#include "Pipeline/EngineDevice.hpp"
#include "Pipeline/Renderer.h"
#include "GameObject.h"
#include "Model.h"
#include <memory>
#include <chrono>
#include "Pipeline/Descriptors.h"
class Engine
{
public:
	Engine() {};
	bool Init();
	void Run();
	void Shutdown();

private:
	
	

	void LoadGameObjects();

	

	void OnResize();

	SDL_Event e;

	unsigned int WINDOW_WIDTH = 640;
	unsigned int WINDOW_HEIGHT = 480;
	const char* WINDOW_TITLE = "RU Vulkan";
	Window window{ WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE };
	bool run = true;
	EngineDevice device{ window };
	Renderer renderer{ window, device, e };


	std::unique_ptr<DescriptorPool> globalPool{};
	GameObject::Map gameObjects;


	
};

