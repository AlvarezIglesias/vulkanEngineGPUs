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
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		VkExtent2D getExtent() { return{ static_cast<uint32_t>(width),static_cast<uint32_t>(height) }; };

	private:

		void initWindow();

		const int width;
		const int height;

		std::string name;
		GLFWwindow* window;

	};


}