#pragma once

#include "glm/glm.hpp"

#include "TowerDelivery/Core/Timestep.h"
#include "TowerDelivery/Scene/CharacterController.h"

namespace TowerDelivery {
	class PlayerCamera {
	public:
		PlayerCamera(CharacterController* characterController);
		~PlayerCamera();

		void OnUpdate(Timestep ts);

		glm::mat4 GetViewMatrix();
		glm::vec3 GetPosition();

	private:
		void updateCameraVectors(Timestep ts);

	private:
		CharacterController* m_pCharacter;

		glm::vec3 m_position;
		glm::vec3 m_front;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;

		glm::vec3 m_characterPosition;
		glm::vec2 m_characterOrientation;

		float m_angle;
		float m_zoom;
		float m_mouseSensitivity;

		float m_previousMouseY;

		bool m_firstUpdate;
	};
}
