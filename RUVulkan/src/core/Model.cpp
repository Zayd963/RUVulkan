#include "Model.h"
#include<cassert>
#include <cstring>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <iostream>
#include "Utils.h"
#define GLM_ENABLE_EXPERIMENTAL
#include<gtx/hash.hpp>
#include <unordered_map>

namespace std
{
	template<>
	struct hash<Model::Vertex>
	{
		size_t operator()(Model::Vertex const& vertex) const {
			size_t seed = 0;
			hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}


Model::Model(EngineDevice& device, const Builder& builder)
	:device{ device }
{
	CreateVertexBuffers(builder.verticies);
	CreateIndexBuffers(builder.indices);
}

Model::~Model()
{
	vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
	vkFreeMemory(device.device(), vertexBufferMemory, nullptr);

	if (hasIndexBuffer)
	{
		vkDestroyBuffer(device.device(), indexBuffer, nullptr);
		vkFreeMemory(device.device(), indexBufferMemory, nullptr);
	}
}

std::unique_ptr<Model> Model::CreateModelFromFile(EngineDevice& device, const std::string& filepath)
{
	Builder builder{};
	builder.LoadModel(filepath);
	std::cout << builder.verticies.size() << std::endl;
	return std::make_unique<Model>(device, builder);
}

void Model::Bind(VkCommandBuffer commandBuffer)
{
	VkBuffer buffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

	if (hasIndexBuffer)
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void Model::Draw(VkCommandBuffer commandBuffer)
{
	if (hasIndexBuffer)
	{
		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
	}
	else
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}
	
}

void Model::CreateVertexBuffers(const std::vector<Vertex>& verticies)
{
	vertexCount = static_cast<uint32_t>(verticies.size());
	assert(vertexCount >= 3 && "Vertex Count Must Be At Least Three");
	VkDeviceSize bufferSize = sizeof(Vertex) * vertexCount;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);


	void* data;
	vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, verticies.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device.device(), stagingBufferMemory);

	device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
	vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
}

void Model::CreateIndexBuffers(const std::vector<uint32_t>& indicies)
{
	indexCount = static_cast<uint32_t>(indicies.size());
	hasIndexBuffer = indexCount > 0;
	
	if (!hasIndexBuffer)
		return;

	VkDeviceSize bufferSize = sizeof(uint32_t) * indexCount;


	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);


	void* data;
	vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indicies.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device.device(), stagingBufferMemory);

	device.createBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	device.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
	vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
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
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

	attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });
	attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) });
	attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) });
	attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv) });


	
	return attributeDescriptions;
}

void Model::Builder::LoadModel(const std::string& filepath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string error;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, filepath.c_str()))
	{
		throw std::runtime_error(warn + error);
	}

	verticies.clear();
	indices.clear();

	std::unordered_map<Vertex, uint32_t> uniqueVerts{};
	for (const auto& shape : shapes)
	{
		for (const auto& Index : shape.mesh.indices)
		{
			Vertex vertex{};
			if (Index.vertex_index >= 0)
			{
				vertex.position = { attrib.vertices[3 * Index.vertex_index + 0], attrib.vertices[3 * Index.vertex_index + 1], attrib.vertices[3 * Index.vertex_index + 2] };

				vertex.color = { attrib.colors[3 * Index.vertex_index + 0], attrib.colors[3 * Index.vertex_index + 1], attrib.colors[3 * Index.vertex_index + 2] , 1.0f};

			}
			if (Index.normal_index >= 0)
			{
				vertex.normal = { attrib.normals[3 * Index.normal_index + 0], attrib.normals[3 * Index.normal_index + 1], attrib.normals[3 * Index.normal_index + 2] };
			}
			if (Index.texcoord_index >= 0)
			{
				vertex.uv = { attrib.texcoords[2 * Index.texcoord_index + 0], attrib.texcoords[2 * Index.texcoord_index + 1]};
			}

			if (uniqueVerts.count(vertex) == 0)
			{
				uniqueVerts[vertex] = static_cast<uint32_t>(verticies.size());
				verticies.push_back(vertex);
			}
			indices.push_back(uniqueVerts[vertex]);
		}
	}
}
