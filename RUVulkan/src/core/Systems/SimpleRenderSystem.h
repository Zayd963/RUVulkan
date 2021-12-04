#pragma once
#include "../Pipeline/Pipeline.h"
#include "../Pipeline/EngineDevice.hpp"
#include "../GameObject.h"
#include "../Pipeline/Camera.h"
#include <memory>
class SimpleRenderSystem
{
public:
	SimpleRenderSystem(EngineDevice& dev, VkRenderPass renderPass);
	~SimpleRenderSystem();
	void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera);

private:
	void CreatePipelineLayout();
	void CreatePipeline(VkRenderPass renderPass);

	EngineDevice& device;

	std::unique_ptr<Pipeline> pipeline;
	
	VkPipelineLayout pipelineLayout;


};



