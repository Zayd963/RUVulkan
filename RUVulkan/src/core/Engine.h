#pragma once
#include "Window/Window.h"
#include "Pipeline/Pipeline.h"
#include "Pipeline/EngineDevice.hpp"
#include "Pipeline/SwapChain.hpp"
#include "Model.h"
#include <memory>
class Engine
{
public:
	Engine() {};
	bool Init();
	void Run();
	void Shutdown();

private:
	void CreatePipelineLayout();
	void CreatePipeline();
	void CreateCommandBuffers();
	void FreeCommandBuffers();
	void DrawFrame();

	void LoadModels();

	void RecreateSwapChain();
	void RecordCommandBuffer(int imageIndex);

	void OnResize();

	SDL_Event e;

	unsigned int WINDOW_WIDTH = 640;
	unsigned int WINDOW_HEIGHT = 480;
	const char* WINDOW_TITLE = "RU Vulkan";
	Window window{ WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE };
	bool run = true;
	EngineDevice device{ window };
	std::unique_ptr<SwapChain> swapChain;
	std::unique_ptr<Pipeline> pipeline;
	std::unique_ptr<Model> model;
	VkPipelineLayout pipelineLayout;
	std::vector<VkCommandBuffer> commandBuffers;
	
};

