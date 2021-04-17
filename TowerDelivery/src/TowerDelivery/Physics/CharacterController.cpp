#include "tdpch.h"
#include "CharacterController.h"

#include "TowerDelivery/KeyCodes.h"
#include "TowerDelivery/MouseButtonCodes.h"
#include "TowerDelivery/Input.h"

#include "TowerDelivery/Log.h"

namespace TowerDelivery {

	CharacterController::CharacterController(float radius, float height, float mass, btVector3 spawnPos, btDiscreteDynamicsWorld* dynamicsWorld)
		:m_DynamicsWorld(dynamicsWorld)
	{

		m_CollisionShape = new btCapsuleShape(radius, height);

		m_MotionState = new btDefaultMotionState(btTransform(btQuaternion(1.0f, 0.0f, 0.0f, 0.0f).normalized(), spawnPos));

		btVector3 inertia;

		m_CollisionShape->calculateLocalInertia(mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, m_MotionState, m_CollisionShape, inertia);

		rigidBodyCI.m_friction = 0.0f;
		rigidBodyCI.m_linearDamping = 0.0f;

		m_RigidBody = new btRigidBody(rigidBodyCI);

		m_RigidBody->setAngularFactor(0.0f);

		m_RigidBody->setActivationState(DISABLE_DEACTIVATION);

		m_DynamicsWorld->addRigidBody(m_RigidBody);
	}

	CharacterController::~CharacterController() {

		m_DynamicsWorld->removeRigidBody(m_RigidBody);

		delete m_MotionState;
		delete m_RigidBody;
	}

	void CharacterController::OnUpdate(Timestep ts){
		UpdatePosition(ts);

		
	}

	void CharacterController::UpdatePosition(Timestep ts){
		
		velocity = 1;


		btTransform transform;
		m_MotionState->getWorldTransform(transform);
		btVector3 location = transform.getOrigin();

		btVector3 vel(m_RigidBody->getLinearVelocity());

		

		if (Input::IsKeyPressed(TD_KEY_UP)) {
			
			vel.setX(vel.x() * +0.5);
			/*
			float x = location.getX();
			x += velocity * ts;
			location.setX(x);
			*/
		}

		transform.setOrigin(location);
		m_MotionState->setWorldTransform(transform);

		//TD_CORE_TRACE("Character Position: {0}, {1}, {2}", float(transform.getOrigin().getX()), float(transform.getOrigin().getY()), float(transform.getOrigin().getZ()));
	
		m_previousPosition = m_RigidBody->getWorldTransform().getOrigin();
	}




}