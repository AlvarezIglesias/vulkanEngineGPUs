

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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

	}

	void  VegWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface");
		}
	}


	void VegWindow::framebufferResizeCallback(GLFWwindow* window, int width, int heigth)
	{
		auto vegWindow = reinterpret_cast<VegWindow*>(glfwGetWindowUserPointer(window));
		vegWindow->frameBufferResized = true;
		vegWindow->width = width;
		vegWindow->height = heigth;
	}

}