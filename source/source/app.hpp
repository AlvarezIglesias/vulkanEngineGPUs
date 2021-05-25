#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "veg_window.hpp"
#include "veg_pipeline.hpp"
#include "veg_device.hpp"
#include "veg_swap_chain.hpp"
#include "veg_game_object.hpp"

#include <memory>
#include <vector>
#include <stdexcept>
#include <array>




namespace veg {


	class App
	{
	public:
		App();
		~App();

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		App(const App&) = delete;
		App& operator = (const App&) = delete;

		void run();


	private:
		void loadGameObjects();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderGameObjects(VkCommandBuffer commandBuffer);

		VegWindow vegWindow{ WIDTH, HEIGHT, "Veg engine" };
		VegDevice vegDevice{ vegWindow };
		std::unique_ptr<VegSwapChain> vegSwapChain;

		std::unique_ptr<VegPipeline> vegPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VegGameObject> gameObjects;
	};
}