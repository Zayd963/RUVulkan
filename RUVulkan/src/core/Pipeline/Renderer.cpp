#include "Renderer.h"
#include <iostream>
#include <array>
#include <stdexcept>
Renderer::Renderer(Window& wind, EngineDevice& dev, SDL_Event& e)
	:window(wind), device(dev), e(e)
{
	RecreateSwapChain();
	CreateCommandBuffers();
}

Renderer::~Renderer()
{
	FreeCommandBuffers();
}

VkCommandBuffer Renderer::BeginFrame()
{
	assert(!isFrameStarted && "Cannot Start Frame When One is Already Started");
	
	auto result = swapChain->acquireNextImage(&currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return nullptr;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		std::cout << "Draw Not Happening" << std::endl;

	isFrameStarted = true;

	auto commandBuffer = GetCurrentCommandBuffer();

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		std::cout << "Command Failed" << std::endl;
	return commandBuffer;
}

void Renderer::EndFrame()
{
	assert(isFrameStarted && "Cannot End Frame If One Is Already Started");

	auto commandBuffer = GetCurrentCommandBuffer();

	

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		std::cout << "Could Not End Buffer" << std::endl;

	auto result = swapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.WasWindowRezied())
	{
		window.ResetWindowResizeFlag();
		RecreateSwapChain();
	}
	if (result != VK_SUCCESS)
		std::cout << "Could Not Submit Command Buffer" << std::endl;

	isFrameStarted = false;

	currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(isFrameStarted && "Cannot Begin RenderPass If One Is Already Started");
	assert(commandBuffer == GetCurrentCommandBuffer() && "Can't Begin RenderPass On Command Buffer From Different Frame");

	



	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain->getRenderPass();
	renderPassInfo.framebuffer = swapChain->getFrameBuffer(currentImageIndex);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.1f, 0.1f , 0.1f , 0.1f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, swapChain->getSwapChainExtent() };
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(isFrameStarted && "Cannot End Frame If One Is Already Started");
	assert(commandBuffer == GetCurrentCommandBuffer() && "Can't End RenderPass On Command Buffer From Different Frame");
	vkCmdEndRenderPass(commandBuffer);
}


void Renderer::CreateCommandBuffers()
{
	commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		std::cout << "Failed to Allocate Command Buffers" << std::endl;
}

void Renderer::FreeCommandBuffers()
{
	vkFreeCommandBuffers(device.device(), device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	commandBuffers.clear();
}

void Renderer::RecreateSwapChain()
{
	auto extent = window.GetExtent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = window.GetExtent();
		SDL_WaitEvent(&e);
	}

	vkDeviceWaitIdle(device.device());

	if (swapChain == nullptr)
	{
		swapChain = std::make_unique<SwapChain>(device, extent);
	}
	else
	{
		std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain);
		swapChain = std::make_unique<SwapChain>(device, extent, oldSwapChain);

		if (!oldSwapChain->CompareFormats(*swapChain.get()))
			throw std::runtime_error("Formats Don't Match");
	}
	std::cout << window.GetExtent().width << " " << window.GetExtent().height << std::endl;
}

void Renderer::OnResize()
{
	window.frameBufferResized = true;
	int w, h;
	SDL_GetWindowSize(window.GetWindow(), &w, &h);
	window.width = w;
	window.height = h;
	std::cout << window.width << " " << window.height << std::endl;
	RecreateSwapChain();
}

