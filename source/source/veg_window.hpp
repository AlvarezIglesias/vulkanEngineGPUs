#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

namespace veg
{

	class VegWindow
	{
	public:
		//width, heigth, name
		VegWindow(int w, int h, std::string n);
		~VegWindow();

		VegWindow(const VegWindow&) = delete;
		VegWindow& operator = (const VegWindow&) = delete;


		bool shouldClose() { return glfwWindowShouldClose(window); };
		VkExtent2D getExtent() { return{ static_cast<uint32_t>(width),static_cast<uint32_t>(height) }; };
		bool wasWindowResized() { return frameBufferResized; }
		void resetWindowResizedFlag() {	frameBufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		GLFWwindow* getWindowGLFW() { return window; }
		

		

	private:

		static void framebufferResizeCallback(GLFWwindow* window, int width, int heigth);

		void initWindow();

		int width;
		int height;

		bool frameBufferResized = false;

		std::string name;
		GLFWwindow* window;

	};


}