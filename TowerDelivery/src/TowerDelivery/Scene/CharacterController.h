#pragma once

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

#include "TowerDelivery/Core/Timestep.h"
#include "glm/glm.hpp"

//Header for a bullet Character Controller; used so that our player character can move throught the world and has Physics applied to them
//enables walking, jumping and rotating of our Character
namespace TowerDelivery {
	class CharacterController
	{
	public:
		CharacterController(float radius, float height, float mass, btVector3 spawnPos, btDiscreteDynamicsWorld* dynamicsWorld);
		~CharacterController();

		void OnUpdate(Timestep ts);

		void Walk(const glm::vec2 dir);
		void Jump();

		glm::vec3 GetPosition();
		void SetPosition(glm::vec3 position);


		glm::vec2 GetOrientation();
		glm::mat4 GetModelMatrix();
		float GetRotation();

	private:
		void UpdatePosition(Timestep ts);
		void UpdateVelocity(Timestep ts);
		void UpdateLookDir();
		inline glm::mat4 btScalar2mat4(btScalar* matrix) {
			return glm::mat4(
				matrix[0], matrix[1], matrix[2], matrix[3],
				matrix[4], matrix[5], matrix[6], matrix[7],
				matrix[8], matrix[9], matrix[10], matrix[11],
				matrix[12], matrix[13], matrix[14], matrix[15]);
		};

		inline btVector3 glm2bt(const glm::vec3& vec)
		{
			return { vec.x, vec.y, vec.z };
		}

	private:
		btCollisionShape* m_pCollisionShape;
		btDefaultMotionState* m_pMotionState;
		btRigidBody* m_pRigidBody;
		btDynamicsWorld* m_pDynamicsWorld;

		btVector3 m_previousPosition;
		btTransform m_motionTransform;

		glm::vec3 m_manualVelocity;

		float m_height;

		float m_maxSpeed;
		float m_deceleration;
		float m_jumpImpulse;
		float m_jumpRechargeTime;
		float m_jumpRechargeTimer;
		bool m_onGround;
		float m_previousMouseX;
		bool m_firstUpdate;
		float m_mouseSensitivity;

		float m_rotation;

		glm::vec2 m_lookDir;
	};
}