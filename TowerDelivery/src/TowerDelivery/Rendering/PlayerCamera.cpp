#include "tdpch.h"
#include "PlayerCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "TowerDelivery/Input.h"

TowerDelivery::PlayerCamera::PlayerCamera(CharacterController* characterController)
	:m_pCharacter(characterController), m_up(glm::vec3(0.0f, 1.0f, 0.0f)), m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_angle(30.0f), m_zoom(10.0f), m_mouseSensitivity(50.0f), m_firstUpdate(true)
{


}

TowerDelivery::PlayerCamera::~PlayerCamera() {
}

void TowerDelivery::PlayerCamera::OnUpdate(Timestep ts) {
	m_characterOrientation = m_pCharacter->GetOrientation();
	m_characterPosition = m_pCharacter->GetPosition();

	if (m_firstUpdate) {
		m_previousMouseY = Input::GetMouseY();
		m_firstUpdate = false;
	}

	updateCameraVectors(ts);
}

glm::mat4 TowerDelivery::PlayerCamera::GetViewMatrix() {
	return glm::lookAt(m_position, m_characterPosition, m_up);
}

glm::vec3 TowerDelivery::PlayerCamera::GetPosition()
{
	return m_position;
}

void TowerDelivery::PlayerCamera::updateCameraVectors(Timestep ts) {
	//update camera angle

	float mouseXOffset = Input::GetMouseY() - m_previousMouseY;
	m_previousMouseY = Input::GetMouseY();

	m_angle += mouseXOffset * m_mouseSensitivity * ts.GetSeconds();

	if (m_angle > 89.0f)
		m_angle = 89.0f;
	if (m_angle < -89.0f)
		m_angle = -89.0f;

	//TD_TRACE(m_angle);


	//update camera position
	glm::vec3 cameraOffsetDirection = -glm::normalize(glm::vec3(m_characterOrientation[0], 0.0f, m_characterOrientation[1]));

	float cameraYOffset = m_zoom * glm::sin(glm::radians(m_angle));
	float cameraXZOffset = m_zoom * glm::cos(glm::radians(m_angle));

	glm::vec3 cameraOffset = cameraOffsetDirection * cameraXZOffset;
	cameraOffset[1] = cameraYOffset;

	m_position = m_characterPosition + cameraOffset;


}
