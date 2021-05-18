#pragma once

#include "veg_window.hpp"
#include "veg_pipeline.hpp"
#include "veg_device.hpp"
#include "veg_swap_chain.hpp"

#include <memory>
#include <vector>
#include <stdexcept>

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

		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		VegWindow vegWindow{ WIDTH, HEIGHT, "Veg engine" };
		VegDevice vegDevice{ vegWindow };
		VegSwapChain vegSwapChain{ vegDevice, vegWindow.getExtent() };

		std::unique_ptr<VegPipeline> vegPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}