#pragma once

#include <glm/glm.hpp>
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

#include "TowerDelivery/Rendering/VertexArray.h"
#include "TowerDelivery/Core/Timestep.h"
#include "TowerDelivery/Rendering/Shader.h"

namespace TowerDelivery {
	class GameObject
	{
	public:

		GameObject(VertexArray* vertexArray, btRigidBody* bulletBody, unsigned int* diffuseTex, unsigned int* specularTex = nullptr);
		~GameObject();

		void OnUpdate();
		void Draw(Shader* shader);

	private:

		VertexArray* m_pVertexArray;
		btRigidBody* m_pBulletBody;

		unsigned int* m_pTextureDiffuse;
		unsigned int* m_pTextureSpecular;

		glm::mat4 m_modelMatrix;

	private:

		glm::mat4 btScalar2mat4(btScalar* matrix) {
			return glm::mat4(
				matrix[0], matrix[1], matrix[2], matrix[3],
				matrix[4], matrix[5], matrix[6], matrix[7],
				matrix[8], matrix[9], matrix[10], matrix[11],
				matrix[12], matrix[13], matrix[14], matrix[15]);
		};

		glm::mat4 getRigidBodyModelMatrix(btRigidBody* body) {
			btTransform trans;
			btScalar btModelMatrix[16];
			glm::mat4 model = glm::mat4(1.0f);

			body->getMotionState()->getWorldTransform(trans);

			trans.getOpenGLMatrix(btModelMatrix);
			model = btScalar2mat4(btModelMatrix);

			return model;
		};
	};

}

