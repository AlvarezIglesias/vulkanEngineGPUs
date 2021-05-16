#pragma once

#include "veg_window.hpp"
#include "veg_pipeline.hpp"

namespace veg {

	class App
	{
	public:
		App();
		~App();

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
		

	private:

		VegWindow vegWindow{ WIDTH, HEIGHT, "Veg engine" };
		VegPipeline vegPipeline{ "shaders/simple_shader.vert.spv","shaders/simple_shader.frag.spv" };

	};
}