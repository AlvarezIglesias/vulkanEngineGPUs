

#include "veg_window.hpp"


namespace veg {

	VegWindow::VegWindow(int w, int h, std::string n): width{w}, height{h}, name{n}
	{
		initWindow();
	}

	VegWindow::~VegWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void VegWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	}

}