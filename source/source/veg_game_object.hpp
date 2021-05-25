#pragma once

#include "veg_model.hpp"

#include <memory>

namespace veg
{

	struct Transform2dComponent
	{
		glm::vec2 translation{};
		glm::vec2 scale{1.f, 1.f };
		float rotation;


		glm::mat2 mat2() { 
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix{ {c, s}, {-s, c}};
			glm::mat2 scaleMat{ {scale.x, 0.0f}, {0.0f, scale.y}};

			return rotMatrix * scaleMat;
		};
	};

	class VegGameObject
	{
	public:

		using id_t = unsigned int;

		static VegGameObject createGameObject() {
			static id_t currentId = 0;
			return VegGameObject{ currentId++ };
		}

		const id_t getId() { return id; }

		VegGameObject(const VegGameObject&) = delete;
		VegGameObject& operator = (const VegGameObject&) = delete;
		VegGameObject(VegGameObject&&) = default;
		VegGameObject& operator = (VegGameObject&&) = default;

		std::shared_ptr<VegModel> model{};
		glm::vec3 color{};
		Transform2dComponent trnasform2d;

	private:
		VegGameObject(id_t objId) : id(objId){}

		id_t id;

	};

}