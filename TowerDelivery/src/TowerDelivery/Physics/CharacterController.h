#pragma once

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

#include "TowerDelivery/Core/Timestep.h"
#include "glm/glm.hpp"

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
		glm::vec2 GetOrientation();
		float GetRotation();

	private:
		void UpdatePosition(Timestep ts);
		void UpdateVelocity(Timestep ts);
		void UpdateLookDir();

	private:
		btCollisionShape* m_pCollisionShape;
		btDefaultMotionState* m_pMotionState;
		btRigidBody* m_pRigidBody;
		btDynamicsWorld* m_pDynamicsWorld;

		btVector3 m_previousPosition;
		btTransform m_motionTransform;

		glm::vec3 m_manualVelocity;

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