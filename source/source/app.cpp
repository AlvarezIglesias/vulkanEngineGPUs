#include "app.hpp"


namespace veg {


	App::App()
	{
		loadModels();
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
			drawFrame();
		}

		vkDeviceWaitIdle(vegDevice.device());
	}

	void App::loadModels()
	{
		std::vector<VegModel::Vertex> vertices{
			{{0.0f, -0.5f}},
			{{0.5f, 0.5f}},
			{{-0.5f, 0.5f}},
			{{0.5f, 0.5f}},
			{{-0.5f, 0.5f}},
			{{1.0f, -1.0f}}
		};

		vegModel = std::make_unique<VegModel>(vegDevice, vertices);

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

	void App::createCommandBuffers(){
	
		commandBuffers.resize(vegSwapChain.imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vegDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(vegDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate command bufer!");
		}

		for (int i = 0; i < commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("Failed to begin recording command bufer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = vegSwapChain.getRenderPass();
			renderPassInfo.framebuffer = vegSwapChain.getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0,0 };
			renderPassInfo.renderArea.extent = vegSwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 0.1f };
			clearValues[1].depthStencil = { 1.0f, 0};

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vegPipeline->bind(commandBuffers[i]);
			vegModel->bind(commandBuffers[i]);
			vegModel->draw(commandBuffers[i]);

			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("Failed to record command buffer!");
			}

		}

	}
	void App::drawFrame(){
	
		uint32_t imageIndex;
		auto result = vegSwapChain.acquireNextImage(&imageIndex);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		result = vegSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}

	}


}