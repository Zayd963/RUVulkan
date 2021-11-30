#pragma once
#include "Pipeline/EngineDevice.hpp"
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
class Model
{
public:

	struct Vertex
	{
		glm::vec2 position;
		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> GetAttirbuteDescriptions();
	};

	Model(EngineDevice& device, const std::vector<Vertex>& verticies);
	~Model();
	Model(const Model&) = delete;
	Model& operator = (const Model&) = delete;

	void Bind(VkCommandBuffer commandBuffer);
	void Draw(VkCommandBuffer commandBuffer);

private:

	void CreateVertexBuffers(const std::vector<Vertex>& verticies);

	EngineDevice& device;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	uint32_t vertexCount;
};
