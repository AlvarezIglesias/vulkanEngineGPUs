#include "app.hpp"


namespace veg {


	App::App()
	{
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	App::~App()
	{
		vkDestroyPipelineLayout(vegDevice.device(), pipelineLayout, nullptr);
	}


	void App::run() {
		while (!vegWindow.shouldClose()) {
			glfwPollEvents();
		}
	}

	void App::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(vegDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void App::createPipeline()
	{
		PipelineConfigInfo pipelineConfig{};
		VegPipeline::defaultPipelineConfigInfo(
			pipelineConfig,
			vegSwapChain.width(),
			vegSwapChain.height());
		pipelineConfig.renderPass = vegSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vegPipeline = std::make_unique<VegPipeline>(vegDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
	}

	void App::createCommandBuffers(){}
	void App::drawFrame(){}


}