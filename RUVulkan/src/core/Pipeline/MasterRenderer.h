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
	void Draw(Camera& sceneCamera, GameObject::Map& gameObjects);

protected:
	
private:
	std::unique_ptr<DescriptorPool> globalPool;// move to renderer
	std::vector<std::unique_ptr<Buffer>> uboBuffers;//move to renderer
	GlobalUBO ubo; // move to renderer
	std::unique_ptr<DescriptorSetLayout> globalSetLayout; // move to renderer
	std::vector<VkDescriptorSet> globalDescriptorSet; // move to renderer
	Renderer& renderer; // move to renderer
	EngineDevice& device;
	std::unique_ptr<SimpleRenderSystem> renderSystem;
};

