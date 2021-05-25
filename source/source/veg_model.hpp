

#pragma once


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include <cassert>
#include "veg_device.hpp"



namespace veg {

	class VegModel
	{
	public:

		struct Vertex {
			glm::vec3 positions;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		VegModel(VegDevice &device, const std::vector<Vertex>& vertices);
		~VegModel();

		VegModel(const VegModel&) = delete;
		VegModel& operator = (const VegModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:

		void createVertexBuffers(const std::vector<Vertex>& vertices);

		VegDevice& vegDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};



}