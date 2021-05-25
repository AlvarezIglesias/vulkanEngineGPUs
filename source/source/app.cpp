#include "app.hpp"


namespace veg {


	struct SimplePushConstantData
	{
		glm::mat2 transform{ 1.f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
		alignas(16) glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	

	App::App()
	{
		loadGameObjects();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}

	App::~App()
	{
		vkDestroyPipelineLayout(vegDevice.device(), pipelineLayout, nullptr);
	}

	void App::processInput(GLFWwindow* window)
	{
			const float cameraSpeed = 0.01f; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	void App::run() {
		while (!vegWindow.shouldClose()) {
			processInput(vegWindow.getWindowGLFW());
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(vegDevice.device());
	}

	void App::loadGameObjects()
	{
		/*std::vector<VegModel::Vertex> vertices{
			{{0.0f, -0.5f, 0.f},{1.0f, 0.0f, 1.0f}},
			{{0.5f, 0.5f, 0.f},{0.0f, 1.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.f},{1.0f, 1.0f, 0.0f}},

			{{0.0f, -0.5f, 0.f},{1.0f, 0.0f, 1.0f}},
			{{0.5f, 0.f, 5.f},{0.0f, 1.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.f},{1.0f, 1.0f, 0.0f}}, 

			{{0.0f, -0.5f, 0.f},{1.0f, 0.0f, 1.0f}},
			{{0.5f, 0.f, 5.f},{0.0f, 1.0f, 1.0f}},
			{{0.5f, 0.5f, 0.f},{1.0f, 1.0f, 0.0f}}
		};*/


		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		std::string path = "modelo.obj";

		std::vector<VegModel::Vertex> vertices{};

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
			throw std::runtime_error(warn + err);
		}

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				auto point1{ attrib.vertices[3 * index.vertex_index + 0] };
				auto point2{ attrib.vertices[3 * index.vertex_index + 1] };
				auto point3{ attrib.vertices[3 * index.vertex_index + 2] };


				vertices.push_back({{point1, point2, point3},{ (index.vertex_index % 100)/100.f, 1.f, 1.f}});

			}
		}


		auto vegModel = std::make_shared<VegModel>(vegDevice, vertices);

		auto triangle = VegGameObject::createGameObject();
		triangle.model = vegModel;
		triangle.color = { 0.1f, 0.8f, 0.1f };
		triangle.trnasform2d.translation.x = 0.2f;
		triangle.trnasform2d.scale = { 2.f, 0.5f };
		triangle.trnasform2d.rotation = 0.25 * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));

	}

	void App::createPipelineLayout()
	{
		/*VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<uint32_t> (vegSwapChain.im.size());*/

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(vegDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void App::createPipeline()
	{
		assert(vegSwapChain != nullptr && "Cannot create pipeline before swap chain");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		VegPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = vegSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vegPipeline = std::make_unique<VegPipeline>(vegDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
	}

	void App::createCommandBuffers(){
	
		commandBuffers.resize(vegSwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vegDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(vegDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate command bufer!");
		}

	}

	void App::recreateSwapChain() {
		auto extent = vegWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = vegWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(vegDevice.device());
		if (vegSwapChain == nullptr) {
			vegSwapChain = std::make_unique<VegSwapChain>(vegDevice, extent);
		}
		else {
			vegSwapChain = std::make_unique<VegSwapChain>(vegDevice, extent, std::move(vegSwapChain));
			if (vegSwapChain->imageCount() != commandBuffers.size()) {
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		createPipeline();
	}

	void App::freeCommandBuffers() {
		vkFreeCommandBuffers(
			vegDevice.device(),
			vegDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data()
		);
		commandBuffers.clear();
	}

	void App::recordCommandBuffer(int imageIndex) {

		/*static int frame = 0;
		frame = (frame + 1) % 1000;*/

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin recording command bufer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vegSwapChain->getRenderPass();
		renderPassInfo.framebuffer = vegSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = vegSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 0.01f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(vegSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(vegSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, vegSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);


		renderGameObjects(commandBuffers[imageIndex]);

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}
	}

	void App::renderGameObjects(VkCommandBuffer commandBuffer) {

		static int frame = 0;

		/*int i = 0;
		for (auto& obj : gameObjects) {
			i++;
			obj.trnasform2d.rotation = glm::mod<float>(obj.trnasform2d.rotation + 0.001f * i, 2.f * glm::pi<float>());
			obj.color = { sin(obj.trnasform2d.rotation + 0.001f * i), 0.2f, cos(obj.trnasform2d.rotation + 0.001f * i) };
		}*/



		vegPipeline->bind(commandBuffer);
		for (auto& obj : gameObjects) {

			

			SimplePushConstantData push{};
			push.offset = obj.trnasform2d.translation;
			push.color = obj.color;
			push.transform = obj.trnasform2d.mat2();

			push.model = glm::rotate(glm::mat4(1.0f),  0.01f * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			push.view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			float aspect = WIDTH / (float)HEIGHT;
			push.proj = glm::perspective(glm::radians(45.0f), atan(tan(100.f / 2) * aspect) * 2 , 0.1f, 1000.0f);
			push.proj[1][1] *= -1;

			vkCmdPushConstants(
				commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);

			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);

		}
	}

	void App::drawFrame(){
	
		uint32_t imageIndex;
		auto result = vegSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		recordCommandBuffer(imageIndex);
		result = vegSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_ERROR_OUT_OF_DATE_KHR || result != VK_SUBOPTIMAL_KHR || vegWindow.wasWindowResized()) {
			vegWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}


		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}

	}


}