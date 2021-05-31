#pragma once

#include <glm/glm.hpp>

#include "CharacterController.h"

namespace TowerDelivery {
	class DetectionArea {
	public:
		DetectionArea(glm::vec3 position, float width, float height, float depth);
		~DetectionArea();

		bool Contains(glm::vec3 position);
		bool Contains(CharacterController* character);

	private:
		glm::vec3 m_position;
		float m_width;
		float m_height;
		float m_depth;
	};
}
