#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


#include "veg_window.hpp"
#include "veg_pipeline.hpp"
#include "veg_device.hpp"
#include "veg_swap_chain.hpp"

#include <memory>
#include <vector>
#include <stdexcept>
#include <array>
#include "veg_model.hpp"



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
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		VegWindow vegWindow{ WIDTH, HEIGHT, "Veg engine" };
		VegDevice vegDevice{ vegWindow };
		std::unique_ptr<VegSwapChain> vegSwapChain;

		std::unique_ptr<VegPipeline> vegPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;

		std::unique_ptr<VegModel> vegModel;
	};
}