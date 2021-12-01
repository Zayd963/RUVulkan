#include "Engine.h"
#include <iostream>
#include <array>

bool Engine::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return false;	
	LoadModels();
	CreatePipelineLayout();
	RecreateSwapChain();
	CreateCommandBuffers();

	return true;
}

void Engine::Run()
{
	while (run)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				run = false;
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				OnResize();
				std::cout << "Window Resized" << std::endl;
			}
		}

		DrawFrame();
	}

	vkDeviceWaitIdle(device.device());
}

void Engine::Shutdown()
{
	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);

	SDL_Quit();
}

void Engine::CreatePipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		std::cout << "Pipeline Layout Creation Failed" << std::endl;
}

void Engine::CreatePipeline()
{
	PipelineConfigInfo pipelineConfig{};
	Pipeline::DefaultConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = swapChain->getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipeline = std::make_unique<Pipeline>(device, pipelineConfig, "res/Shaders/Basic_shader.vert.spv", "res/Shaders/Basic_shader.frag.spv");
}

void Engine::CreateCommandBuffers()
{
	commandBuffers.resize(swapChain->imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		std::cout << "Failed to Allocate Command Buffers" << std::endl;
}

void Engine::FreeCommandBuffers()
{
	vkFreeCommandBuffers(device.device(), device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	commandBuffers.clear();
}

void Engine::DrawFrame()
{
	uint32_t imageIndex;
	auto result = swapChain->acquireNextImage(&imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		std::cout << "Draw Not Happening" << std::endl;

	RecordCommandBuffer(imageIndex);

	result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.WasWindowRezied())
	{
		window.ResetWindowResizeFlag();
		RecreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS)
		std::cout << "Could Not Submit Command Buffer" << std::endl;
}

void Engine::LoadModels()
{
	std::vector<Model::Vertex> verts
	{
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}}
	};

	model = std::make_unique<Model>(device, verts);
}

void Engine::RecreateSwapChain()
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
		//swapChain = nullptr;
		swapChain = std::make_unique<SwapChain>(device, extent, std::move(swapChain));
		if (swapChain->imageCount() != commandBuffers.size())
		{
			FreeCommandBuffers();
			CreateCommandBuffers();
		}
	}
	CreatePipeline();

	std::cout << window.GetExtent().width << " " << window.GetExtent().height << std::endl;
}

void Engine::RecordCommandBuffer(int imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
		std::cout << "Command Failed" << std::endl;

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain->getRenderPass();
	renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.1f, 0.1f , 0.1f , 0.1f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, swapChain->getSwapChainExtent() };
	vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);


	pipeline->Bind(commandBuffers[imageIndex]);
	model->Bind(commandBuffers[imageIndex]);
	model->Draw(commandBuffers[imageIndex]);


	vkCmdDraw(commandBuffers[imageIndex], 3, 1, 0, 0);

	vkCmdEndRenderPass(commandBuffers[imageIndex]);

	if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
		std::cout << "Could Not End Buffer" << std::endl;
}

void Engine::OnResize()
{
	window.frameBufferResized = true;
	int w, h;
	SDL_GetWindowSize(window.GetWindow(), &w, &h);
	window.width = w;
	window.height = h;
	std::cout << window.width << " " << window.height << std::endl;
	RecreateSwapChain();
}
