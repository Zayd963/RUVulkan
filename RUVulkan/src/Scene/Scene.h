#pragma once
#include "../core/Pipeline/EngineDevice.hpp"
#include "../core/Window/Window.h"
#include "../core/Pipeline/Renderer.h"
#include "../core/Pipeline/Camera.h"
#include "../core/Pipeline/Descriptors.h"
#include "../core/Pipeline/Buffer.h"
#include "../core/Systems/SimpleRenderSystem.h"
#include "../core/Pipeline/SwapChain.hpp"
#include "../core/Pipeline/FrameInfo.h"
#include <vector>
class Scene
{
public:
	Scene(EngineDevice &dev, Window& window, Renderer& renderer);
	virtual void Update(float frameTime);
	virtual void Render();
	Camera& GetCamera() { return sceneCamera; }
private:
	void LoadGameObjects(EngineDevice& device);
	
	EngineDevice& device;
	Camera sceneCamera{};
	std::unique_ptr<DescriptorPool> globalPool;
	std::vector<std::unique_ptr<Buffer>> uboBuffers;
	GlobalUBO ubo{};
	std::unique_ptr<DescriptorSetLayout> globalSetLayout;
	std::vector<VkDescriptorSet> globalDescriptorSet;
	Renderer& renderer;
	GameObject::Map gameObjects;
	std::unique_ptr<SimpleRenderSystem> renderSystem;
};

