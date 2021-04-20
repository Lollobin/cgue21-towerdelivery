#pragma once

#include "tdpch.h"
#include "TowerDelivery/Core/Timestep.h"
#include "glm/glm.hpp"

namespace TowerDelivery {
	// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
	class Camera
	{
	public:
		// camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		// euler Angles
		float Yaw;
		float Pitch;
		float lastX;
		float lastY;
		// camera options
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

	public:
		// constructor with vectors
		Camera(glm::vec3 position);

		// constructor with scalar values
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	
		// returns the view matrix calculated using Euler Angles and the LookAt Matrix
		glm::mat4 GetViewMatrix();

		void OnUpdate(Timestep ts);

		// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll(float yoffset);

	private:
		// calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors();
	};
}