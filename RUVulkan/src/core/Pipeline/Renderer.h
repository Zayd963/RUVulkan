#pragma once
#include "../Window/Window.h"
#include "../Pipeline/EngineDevice.hpp"
#include "../Pipeline/SwapChain.hpp"
#include "../Model.h"
#include <memory>
#include <cassert>
class Renderer
{
public:
	Renderer(Window& wind, EngineDevice& dev);
	~Renderer();
	VkCommandBuffer BeginFrame();
	void EndFrame();

	void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

	bool IsFrameStarted() const { return isFrameStarted; }

	VkCommandBuffer GetCurrentCommandBuffer() const 
	{
		assert(isFrameStarted && "No Command Buffer if Frame Has Not Started");
		return commandBuffers[currentFrameIndex];
	}
	VkRenderPass GetSwapChainRenderPass() const
	{
		return swapChain->getRenderPass();
	}

	float GetSwapChainAspectRatio() const { return swapChain->extentAspectRatio(); }
	float GetSwapChainWidth() const { return swapChain->width(); }
	float GetSwapChainHeight() const { return swapChain->height(); }
	int GetCurrentFrameIndex() const
	{
		assert(isFrameStarted && "No Frame Index if Frame Has Not Started");
		return currentFrameIndex;
	}

	void OnResize();
private:
	
	void CreateCommandBuffers();
	void FreeCommandBuffers();

	uint32_t currentImageIndex{ 0 };
	int currentFrameIndex{ 0 };
	bool isFrameStarted{ false };

	void RecreateSwapChain();
	
	Window& window;
	EngineDevice& device;
	std::unique_ptr<SwapChain> swapChain;
	std::vector<VkCommandBuffer> commandBuffers;

};



