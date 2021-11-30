#include "Model.h"
#include<cassert>
#include <cstring>
Model::Model(EngineDevice& device, const std::vector<Vertex>& verticies)
	:device{ device }
{
	CreateVertexBuffers(verticies);
}

Model::~Model()
{
	vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
	vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
}

void Model::Bind(VkCommandBuffer commandBuffer)
{
	VkBuffer buffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void Model::Draw(VkCommandBuffer commandBuffer)
{
	vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

void Model::CreateVertexBuffers(const std::vector<Vertex>& verticies)
{
	vertexCount = static_cast<uint32_t>(verticies.size());
	assert(vertexCount >= 3 && "Vertex Count Must Be At Least Three");
	VkDeviceSize bufferSize = sizeof(Vertex) * vertexCount;
	device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);
	void* data;
	vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, verticies.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device.device(), vertexBufferMemory);
}

std::vector<VkVertexInputBindingDescription> Model::Vertex::GetBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions(1);
	vertexBindingDescriptions[0].binding = 0;
	vertexBindingDescriptions[0].stride = sizeof(Vertex);
	vertexBindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return vertexBindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::GetAttirbuteDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = 0;
	return attributeDescriptions;
}
