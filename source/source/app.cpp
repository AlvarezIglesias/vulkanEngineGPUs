

#include "app.hpp"

namespace veg {


	App::App()
	{
	}

	App::~App()
	{
	}


	void App::run() {
		while (!vegWindow.shouldClose()) {
			glfwPollEvents();
		}
	}


}