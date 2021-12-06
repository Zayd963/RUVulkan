#include "SimpleRenderSystem.h"
#include <iostream>
#include <array>
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <gtc/constants.hpp>
struct SimplePushConstantData
{
	glm::mat4 normalMatrix{ 1.0f };
	glm::mat4 modelMatrix{ 1.0f };
};

SimpleRenderSystem::SimpleRenderSystem(EngineDevice& dev, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout)
	:device(dev)
{
	CreatePipelineLayout(globalDescriptorSetLayout);
	CreatePipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout globalDescriptorSetLayout)
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalDescriptorSetLayout };

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
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



void SimpleRenderSystem::RenderGameObjects(FrameInfo& frameInfo)
{
	pipeline->Bind(frameInfo.commandBuffer);

	vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
		0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

	for (auto& kv : frameInfo.gameObjects)
	{
		auto& obj = kv.second;
		
		SimplePushConstantData push{};
		auto modelMatrix = obj.transform.Mat4();
		push.normalMatrix = obj.transform.NormalMatrix();
		push.modelMatrix = obj.transform.Mat4();

		vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0, sizeof(SimplePushConstantData), &push);
		obj.model->Bind(frameInfo.commandBuffer);
		obj.model->Draw(frameInfo.commandBuffer);
	}
}







