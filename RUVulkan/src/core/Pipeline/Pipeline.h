#pragma once
#include <vector>
#include "EngineDevice.hpp"

struct PipelineConfigInfo
{
	PipelineConfigInfo(const PipelineConfigInfo&) = delete;
	PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineViewportStateCreateInfo viewportInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;

};

class Pipeline
{
public:
	Pipeline(EngineDevice& device, const PipelineConfigInfo &config, const char* _vertFilepath, const char* _fragFilepath);
	Pipeline(const Pipeline&) = delete;
	Pipeline operator = (const Pipeline&) = delete;
	~Pipeline();
	static void DefaultConfigInfo(PipelineConfigInfo& info, uint32_t width, uint32_t height);
	void Bind(VkCommandBuffer commandBuffer);

private:
	static std::vector<char> ReadFile(const char* _filepath);
	void CreateGraphicsPipeline(const char* _vertFilepath, const char* _fragFilepath, const PipelineConfigInfo &info);
	void CreateShaderModule(std::vector<char> code, VkShaderModule* shaderModule);
	EngineDevice& device;
	VkPipeline graphicsPipeline;

	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
};

