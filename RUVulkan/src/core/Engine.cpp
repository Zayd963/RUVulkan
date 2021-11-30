#include "Engine.h"
#include <iostream>
#include <array>

bool Engine::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return false;	

	CreatePipelineLayout();
	CreatePipeline();
	CreateCommandBuffers();

	return true;
}

void Engine::Run()
{
	SDL_Event e;
	while (run)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				run = false;
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
	Pipeline::DefaultConfigInfo(pipelineConfig, swapChain.width(), swapChain.height());
	pipelineConfig.renderPass = swapChain.getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipeline = std::make_unique<Pipeline>(device, pipelineConfig, "res/Shaders/Basic_shader.vert.spv", "res/Shaders/Basic_shader.frag.spv");
}

void Engine::CreateCommandBuffers()
{
	commandBuffers.resize(swapChain.imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		std::cout << "Failed to Allocate Command Buffers" << std::endl;

	for (int i = 0; i < commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
			std::cout << "Command Failed" << std::endl;

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapChain.getRenderPass();
		renderPassInfo.framebuffer = swapChain.getFrameBuffer(i);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f , 0.1f , 0.1f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		pipeline->Bind(commandBuffers[i]);
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
			std::cout << "Could Not End Buffer" << std::endl;
	}
}

void Engine::DrawFrame()
{
	uint32_t imageIndex;
	auto result = swapChain.acquireNextImage(&imageIndex);
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		std::cout << "Draw Not Happening" << std::endl;

	result = swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if (result != VK_SUCCESS)
		std::cout << "Could Not Submit Command Buffer" << std::endl;
}
