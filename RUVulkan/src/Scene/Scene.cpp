#include "Scene.h"
#include <memory>
#include <iostream>

Scene::Scene(EngineDevice& dev, Window& window, Renderer& renderer)
	:device(dev), renderer(renderer), globalSetLayout{ DescriptorSetLayout::Builder(dev).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL).build() }
{
	globalPool = DescriptorPool::Builder(dev).setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		SwapChain::MAX_FRAMES_IN_FLIGHT).build();

	uboBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
	for (int i = 0; i < uboBuffers.size(); i++)
	{
		uboBuffers[i] = std::make_unique<Buffer>(dev, sizeof(GlobalUBO), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, dev.properties.limits.minUniformBufferOffsetAlignment);

		uboBuffers[i]->map();
	}

	globalSetLayout = DescriptorSetLayout::Builder(dev).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL).build();

	globalDescriptorSet.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
	for (int i = 0; i < globalDescriptorSet.size(); i++)
	{
		auto bufferInfo = uboBuffers[i]->descriptorInfo();
		DescriptorWriter(*globalSetLayout, *globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSet[i]);
	}

	renderSystem = std::make_unique<SimpleRenderSystem>(device, renderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());

	float aspect = renderer.GetSwapChainAspectRatio();
	sceneCamera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);
	sceneCamera.SetViewTarget(glm::vec3{ -1.f, -2.f, 2.f }, glm::vec3{ 0.f, 0.f, 2.5f });
	LoadGameObjects(dev);
}

float light = 0;
float increment = 1.f;
void Scene::Update(float frameTime)
{
	if (light > 2)
		increment = -1;
	else if (light < -2)
		increment = 1;

	light += increment * frameTime;
	ubo.lightDirection.x = light;
	
}

void Scene::Render()
{
	if (auto commandBuffer = renderer.BeginFrame())
	{
		int frameIndex = renderer.GetCurrentFrameIndex();
		FrameInfo info{ frameIndex, 0, commandBuffer, sceneCamera, globalDescriptorSet[frameIndex], gameObjects };
		ubo.projectionView = sceneCamera.GetProjection() * sceneCamera.GetView();

		uboBuffers[frameIndex]->writeToBuffer(&ubo);
		uboBuffers[frameIndex]->flushIndex(frameIndex);

		renderer.BeginSwapChainRenderPass(commandBuffer);
		renderSystem->RenderGameObjects(info);
		renderer.EndSwapChainRenderPass(commandBuffer);
		renderer.EndFrame();
	}
}

void Scene::LoadGameObjects(EngineDevice& device)
{

	std::shared_ptr<Model> flatVase = Model::CreateModelFromFile(device, "res/Models/flat_vase.obj");

	auto gameObject1 = GameObject::CreateGameObject();
	gameObject1.model = flatVase;
	gameObject1.transform.translation = { 0.f, 0.f, 0.f };
	gameObject1.transform.scale = glm::vec3{ 3.f, 1.5f, 3.f };
	gameObjects.emplace(gameObject1.GetID(), std::move(gameObject1));

	std::shared_ptr<Model> smoothVase = Model::CreateModelFromFile(device, "res/Models/smooth_vase.obj");

	auto gameObject = GameObject::CreateGameObject();
	gameObject.model = smoothVase;
	gameObject.transform.translation = { -1.f, 0.f, 0.f };
	gameObject.transform.scale = glm::vec3{ 3.f, 1.5f, 3.f };
	gameObjects.emplace(gameObject.GetID(), std::move(gameObject));

	std::shared_ptr<Model> Quad = Model::CreateModelFromFile(device, "res/Models/Quad.obj");

	auto floor = GameObject::CreateGameObject();
	floor.model = Quad;
	floor.transform.translation = { 0.f, 0.f, 0.f };
	floor.transform.scale = glm::vec3{ 3.f, 1.f, 3.f };
	gameObjects.emplace(floor.GetID(), std::move(floor));
}
