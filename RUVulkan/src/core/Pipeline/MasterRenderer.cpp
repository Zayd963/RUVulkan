#include "MasterRenderer.h"

MasterRenderer::MasterRenderer(EngineDevice& dev, Renderer& renderer)
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
}

void MasterRenderer::Draw(Camera& sceneCamera, GameObject::Map& gameObjects, GlobalUBO& ubo)
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


