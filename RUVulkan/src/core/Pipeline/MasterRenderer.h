#pragma once
#include "../GameObject.h"
#include "../Systems/SimpleRenderSystem.h"
#include "Renderer.h"
#include "Descriptors.h"
#include <vector>
#include <memory>
class MasterRenderer
{
public:
	MasterRenderer(EngineDevice& dev, Renderer& renderer);
	void Draw(Camera& sceneCamera, GameObject::Map& gameObjects, GlobalUBO& ubo);

protected:
	
private:
	std::unique_ptr<DescriptorPool> globalPool;
	std::vector<std::unique_ptr<Buffer>> uboBuffers;

	std::unique_ptr<DescriptorSetLayout> globalSetLayout; 
	std::vector<VkDescriptorSet> globalDescriptorSet; 
	Renderer& renderer; 
	EngineDevice& device;
	std::unique_ptr<SimpleRenderSystem> renderSystem;
};

