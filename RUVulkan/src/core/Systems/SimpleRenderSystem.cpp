#include "SimpleRenderSystem.h"
#include <iostream>
#include <array>
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <gtc/constants.hpp>
struct SimplePushConstantData
{
	glm::mat4 transfrom{ 1.0f };
	alignas(16) glm::vec4 color;
};

SimpleRenderSystem::SimpleRenderSystem(EngineDevice& dev, VkRenderPass renderPass)
	:device(dev)
{
	CreatePipelineLayout();
	CreatePipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::CreatePipelineLayout()
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		std::cout << "Pipeline Layout Creation Failed" << std::endl;
}

void SimpleRenderSystem::CreatePipeline(VkRenderPass renderPass)
{
	PipelineConfigInfo pipelineConfig{};
	Pipeline::DefaultConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipeline = std::make_unique<Pipeline>(device, pipelineConfig, "res/Shaders/Basic_shader.vert.spv", "res/Shaders/Basic_shader.frag.spv");
}



void SimpleRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera)
{
	pipeline->Bind(commandBuffer);
	auto projView = camera.GetProjection() * camera.GetView();
	for (auto& obj : gameObjects)
	{
		obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.001f, glm::two_pi<float>());
		obj.transform.rotation.x = glm::mod((obj.transform.rotation.x + 0.001f) , glm::two_pi<float>());
		SimplePushConstantData push{};
		push.color = obj.color;
		push.transfrom = projView * obj.transform.Mat4();

		vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0, sizeof(SimplePushConstantData), &push);
		obj.model->Bind(commandBuffer);
		obj.model->Draw(commandBuffer);
	}
}







