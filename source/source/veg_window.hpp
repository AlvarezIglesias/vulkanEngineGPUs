#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace veg
{

	class VegWindow
	{
	public:
		//width, heigth, name
		VegWindow(int w, int h, std::string n);
		~VegWindow();

		bool shouldClose() { return glfwWindowShouldClose(window); };

	private:

		void initWindow();

		const int width;
		const int height;

		std::string name;
		GLFWwindow* window;

	};


}