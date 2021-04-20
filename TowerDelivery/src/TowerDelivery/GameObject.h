#pragma once

#include <glm/glm.hpp>
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

namespace TowerDelivery {
	class GameObject
	{
	public:

		GameObject();
		~GameObject();

		void OnUpdate();


	private:

		glm::mat4 btScalar2mat4(btScalar* matrix) {
			return glm::mat4(
				matrix[0], matrix[1], matrix[2], matrix[3],
				matrix[4], matrix[5], matrix[6], matrix[7],
				matrix[8], matrix[9], matrix[10], matrix[11],
				matrix[12], matrix[13], matrix[14], matrix[15]);
		};
	};

}

