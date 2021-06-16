#include "tdpch.h"
#include "CharacterController.h"

#include "TowerDelivery/Core/KeyCodes.h"
#include "TowerDelivery/Core/MouseButtonCodes.h"
#include "TowerDelivery/Core/Input.h"

#include "TowerDelivery/Core/Log.h"
#include "glm/gtc/matrix_transform.hpp"

//Character Controller Class, implements CharacterController.h
namespace TowerDelivery {
	//Constructor, that takes radius, height, mass, spawn Position and which Wolrd we are in and sets speed, deceleration, jump impuls, rotation and mouse Sensivity to fixed values
	CharacterController::CharacterController(float radius, float height, float mass, btVector3 spawnPos, btDiscreteDynamicsWorld* dynamicsWorld)
		:m_pDynamicsWorld(dynamicsWorld), m_maxSpeed(10.0f), m_deceleration(7.0f), m_manualVelocity(0.0f, 0.0f, 0.0f),
		m_jumpImpulse(720.0f), m_jumpRechargeTime(0.1f), m_jumpRechargeTimer(0.0f), m_onGround(false), m_lookDir(glm::vec2(0.0f, 0.0f)),
		m_rotation(0.1f), m_firstUpdate(true), m_mouseSensitivity(20.0f), m_height(height), m_previousMouseX(0.0f)
	{
		m_pCollisionShape = new btBoxShape(btVector3(0.84f / 2.0f, height / 2.0f, 0.64f / 2.0f));

		m_pMotionState = new btDefaultMotionState(btTransform(btQuaternion(1.0f, 0.0f, 0.0f, 0.0f).normalized(), spawnPos));

		btVector3 inertia;

		m_pCollisionShape->calculateLocalInertia(mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, m_pMotionState, m_pCollisionShape, inertia);

		rigidBodyCI.m_friction = 0.0f;
		rigidBodyCI.m_linearDamping = 0.0f;

		m_pRigidBody = new btRigidBody(rigidBodyCI);

		m_pRigidBody->setAngularFactor(0.0f);

		m_pRigidBody->setActivationState(DISABLE_DEACTIVATION);

		m_pDynamicsWorld->addRigidBody(m_pRigidBody);
	}

	//deconstructor for deleting the Character Controller
	CharacterController::~CharacterController() {
		m_pDynamicsWorld->removeRigidBody(m_pRigidBody);

		delete m_pMotionState;
		delete m_pRigidBody;
	}

	//sets Keybinds for walking and jumping lets the Player control which way the character faces by mouse position
	void CharacterController::OnUpdate(Timestep ts) {
		//update Transform
		m_pMotionState->getWorldTransform(m_motionTransform);

		//check if character is on ground
		btVector3 t_playerPos = m_pRigidBody->getWorldTransform().getOrigin();
		btVector3 t_groundPos = t_playerPos - btVector3(0.0f, m_height / 2.0f + 0.01f, 0.0f);
		btCollisionWorld::ClosestRayResultCallback res(t_playerPos, t_groundPos);

		m_pDynamicsWorld->rayTest(t_playerPos, t_groundPos, res);

		float speedFactor = 60.0f * ts;

		if (res.hasHit()) {
			//TD_TRACE("floor");
			m_onGround = true;
			//m_jumpRechargeTimer = m_jumpRechargeTime;
		}
		else {
			//TD_TRACE("air");
			m_onGround = false;
			speedFactor = speedFactor * 0.5f;
		}

		//check arrow keys for walking
		if (Input::IsKeyPressed(TD_KEY_W))
			Walk(m_lookDir * speedFactor);
		if (Input::IsKeyPressed(TD_KEY_S))
			Walk(glm::vec2(-m_lookDir[0], -m_lookDir[1]) * speedFactor);
		if (Input::IsKeyPressed(TD_KEY_A))
			Walk(glm::vec2(m_lookDir[1], -m_lookDir[0]) * speedFactor);
		if (Input::IsKeyPressed(TD_KEY_D))
			Walk(glm::vec2(-m_lookDir[1], m_lookDir[0]) * speedFactor);
		if (Input::IsKeyPressed(TD_KEY_SPACE))
			Jump();

		//update rotation based on mouse X
		if (m_firstUpdate) {
			m_previousMouseX = Input::GetMouseX();
			m_firstUpdate = false;
		}

		m_rotation += (m_previousMouseX - Input::GetMouseX()) * ts.GetSeconds() * m_mouseSensitivity;

		if (m_rotation > 360.0f || m_rotation < -360.0f)
			m_rotation = 0.001;

		m_previousMouseX = Input::GetMouseX();

		//TD_TRACE("Rotation: {0}", m_rotation);

		UpdatePosition(ts);
		UpdateVelocity(ts);
		UpdateLookDir();

		// Update jump timer
		if (m_jumpRechargeTimer < m_jumpRechargeTime)
			m_jumpRechargeTimer += ts.GetSeconds();

		//TD_TRACE("{0}", m_jumpRechargeTimer);
	}

	void CharacterController::Walk(const glm::vec2 dir)
	{
		glm::vec2 velocityXZ(dir + glm::vec2(m_manualVelocity.x, m_manualVelocity.z));

		float speedXZ = float(velocityXZ.length()) * 1.5f;

		if (speedXZ > m_maxSpeed)
			velocityXZ = velocityXZ / speedXZ * m_maxSpeed;

		m_manualVelocity.x = velocityXZ[0];
		m_manualVelocity.z = velocityXZ[1];
	}

	void CharacterController::Jump()
	{
		if (m_onGround && m_jumpRechargeTimer >= m_jumpRechargeTime)
		//if (m_onGround)
		{
			m_jumpRechargeTimer = 0.0f;
			m_pRigidBody->applyCentralImpulse(btVector3(0.0f, m_jumpImpulse, 0.0f));

			// Move upwards slightly so velocity isn't immediately canceled when it detects it as on ground next frame
			const float jumpYOffset = 0.0f;

			//float previousY = m_pRigidBody->getWorldTransform().getOrigin().getY();

			//m_pRigidBody->getWorldTransform().getOrigin().setY(previousY + jumpYOffset);
		}
	}

	glm::vec3 CharacterController::GetPosition() {
		btVector3 position = m_motionTransform.getOrigin();
		return glm::vec3(position.x(), position.y(), position.z());
	}

	void CharacterController::SetPosition(glm::vec3 position)
	{
		m_pRigidBody->getWorldTransform().getOrigin().setValue(position.x, position.y, position.z);
	}

	glm::vec2 CharacterController::GetOrientation()
	{
		return m_lookDir;
	}

	glm::mat4 CharacterController::GetModelMatrix()
	{
		glm::mat4 model = glm::mat4(1.0f);
		btTransform trans;
		btScalar btModelMatrix[16];

		m_pMotionState->getWorldTransform(trans);
		trans.getOpenGLMatrix(btModelMatrix);

		model = btScalar2mat4(btModelMatrix);

		model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, -1.0f, 0.0f));

		return model;
	}

	float CharacterController::GetRotation()
	{
		return m_rotation;
	}

	void CharacterController::UpdatePosition(Timestep ts) {
	}

	void CharacterController::UpdateVelocity(Timestep ts)
	{
		//adjust only xz velocity
		m_manualVelocity.y = m_pRigidBody->getLinearVelocity().getY();

		//set velocity
		m_pRigidBody->setLinearVelocity(btVector3(m_manualVelocity.x, m_manualVelocity.y, m_manualVelocity.z));

		if (m_onGround) {
			m_manualVelocity -= m_manualVelocity * m_deceleration * ts.GetSeconds();
		}
		else {
			m_manualVelocity -= m_manualVelocity * m_deceleration * 0.7f * ts.GetSeconds();
		}
	}

	void CharacterController::UpdateLookDir() {
		//calculate x
		m_lookDir[0] = -glm::sin(glm::radians(m_rotation));

		//calculate z
		m_lookDir[1] = -glm::cos(glm::radians(m_rotation));
		//TD_TRACE(m_lookDir[1]);
	}
}