#pragma once

#include "TowerDelivery.h"

enum ContainerColor {
	black, blue, green, grey, orange, red, white
};

enum ContainerModel {
	model1, model2, model3, model4
};

class ContainerObject
{
public:
	ContainerObject(TowerDelivery::Shader* shader,
		btDiscreteDynamicsWorld* dynamicsWorld,
		ContainerModel modelNum,
		TowerDelivery::Model* model,
		glm::vec3 position,
		glm::vec3 rotation,
		unsigned int albedo,
		unsigned int normal,
		unsigned int metallic,
		unsigned int roughness,
		unsigned int ao);

	~ContainerObject();

	void Draw();

	inline glm::mat4 btScalar2mat4(btScalar* matrix) {
		return glm::mat4(
			matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[12], matrix[13], matrix[14], matrix[15]);
	};

private:
	btRigidBody* m_pBulletBody;
	TowerDelivery::Model* m_pModel;
	TowerDelivery::Shader* m_pShader;
	glm::mat4 m_modelMatrix;

	ContainerColor m_color;
	glm::vec3 m_size;

	unsigned int t_albedo;
	unsigned int t_normal;
	unsigned int t_metallic;
	unsigned int t_roughness;
	unsigned int t_ao;
};
