#pragma once
#include "Pipeline/EngineDevice.hpp"
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <memory>
#include "Pipeline/Buffer.h"
class Model
{
public:

	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec3 normal;
		glm::vec2 uv;
		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> GetAttirbuteDescriptions();

		bool operator == (const Vertex& other) const
		{
			return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
		}

	};

	struct Builder
	{
		std::vector<Vertex> verticies{};
		std::vector<uint32_t> indices{};

		void LoadModel(const std::string& filepath);
	};

	Model(EngineDevice& device, const Builder& builder);
	~Model();
	Model(const Model&) = delete;
	Model& operator = (const Model&) = delete;

	static std::unique_ptr<Model> CreateModelFromFile(EngineDevice& device, const std::string& filepath);

	void Bind(VkCommandBuffer commandBuffer);
	void Draw(VkCommandBuffer commandBuffer);

private:

	void CreateVertexBuffers(const std::vector<Vertex>& verticies);
	void CreateIndexBuffers(const std::vector<uint32_t>& indicies);

	EngineDevice& device;
	std::unique_ptr<Buffer> vertexBuffer;
	uint32_t vertexCount;

	std::unique_ptr<Buffer> indexBuffer;
	uint32_t indexCount;
	bool hasIndexBuffer{ false };
};

