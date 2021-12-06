#pragma once
#include "../Pipeline/Pipeline.h"
#include "../Pipeline/EngineDevice.hpp"
#include "../GameObject.h"
#include "../Pipeline/Camera.h"
#include <memory>
#include "../Pipeline/FrameInfo.h"
class SimpleRenderSystem
{
public:
	SimpleRenderSystem(EngineDevice& dev, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout);
	~SimpleRenderSystem();
	void RenderGameObjects(FrameInfo& frameInfo);

private:
	void CreatePipelineLayout(VkDescriptorSetLayout globalDescriptorSetLayout);
	void CreatePipeline(VkRenderPass renderPass);

	EngineDevice& device;

	std::unique_ptr<Pipeline> pipeline;
	
	VkPipelineLayout pipelineLayout;


};



