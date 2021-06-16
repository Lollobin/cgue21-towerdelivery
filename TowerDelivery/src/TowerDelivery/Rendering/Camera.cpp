#include "tdpch.h"
#include "Camera.h"
#include "TowerDelivery/Core/KeyCodes.h"
#include "TowerDelivery/Core/MouseButtonCodes.h"
#include "TowerDelivery/Core/Input.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const float YAW = 45.0f;
const float PITCH = 0.0f;
const float SPEED = 7.0f;
const float SENSITIVITY = 7.0f;
const float ZOOM = 45.0f;

namespace TowerDelivery {

	Camera::Camera(glm::vec3 position) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		Yaw = YAW;
		Pitch = PITCH;
		updateCameraVectors();
	}

	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void Camera::OnUpdate(Timestep ts) {
		float velocity = MovementSpeed * ts;

		if (Input::IsKeyPressed(TD_KEY_UP))
			Position += Front * velocity;

		if (Input::IsKeyPressed(TD_KEY_DOWN))
			Position -= Front * velocity;

		if (Input::IsKeyPressed(TD_KEY_LEFT))
			Position -= Right * velocity;

		if (Input::IsKeyPressed(TD_KEY_RIGHT))
			Position += Right * velocity;

		if (Input::IsKeyPressed(TD_KEY_RIGHT_SHIFT))
			Position += Up * velocity;

		if (Input::IsKeyPressed(TD_KEY_RIGHT_CONTROL))
			Position -= Up * velocity;

		if (!Input::IsMouseButtonPressed(TD_MOUSE_BUTTON_LEFT)) {
			lastX = Input::GetMouseX();
			lastY = Input::GetMouseY();
		}

		float xoffset = Input::GetMouseX() - lastX;
		float yoffset = lastY - Input::GetMouseY();

		lastX = Input::GetMouseX();
		lastY = Input::GetMouseY();

		Yaw += xoffset * MouseSensitivity * ts;
		Pitch += yoffset * MouseSensitivity * ts;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (true)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void Camera::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	// calculates the front vector from the Camera's (updated) Euler Angles
	void Camera::updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Camera::Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
}