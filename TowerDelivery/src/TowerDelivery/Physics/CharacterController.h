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


	private:
		btCollisionShape* m_CollisionShape;
		btDefaultMotionState* m_MotionState;
		btRigidBody* m_RigidBody;
		btDynamicsWorld* m_DynamicsWorld;

		btVector3 m_previousPosition;

		btTransform m_motionTransform;

		float velocity;

		void UpdatePosition(Timestep ts);
		void UpdateVelocity(Timestep ts);

	};

}