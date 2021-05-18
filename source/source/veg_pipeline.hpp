#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "veg_device.hpp"

namespace veg {

	struct PipelineConfigInfo {

		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class VegPipeline
	{
	public:
		VegPipeline(VegDevice &device, const std::string &vertFilepath, const std::string & fragFilepath, const PipelineConfigInfo &configInfo);
		~VegPipeline();

		VegPipeline(const VegPipeline&) = delete;
		void operator=(const VegPipeline&) = delete;

		static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo,uint32_t width, uint32_t height);

	private:
		
		static std::vector<char> readFile(const std::string& filepath);

		void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector <char>& code, VkShaderModule* shaderModule);

		VegDevice& vegDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

	};

}