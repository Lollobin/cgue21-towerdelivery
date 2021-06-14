#include "ContainerObject.h"

ContainerObject::ContainerObject(TowerDelivery::Shader* shader, btDiscreteDynamicsWorld* dynamicsWorld,
	ContainerModel modelNum, TowerDelivery::Model* model, ContainerColor color, glm::vec3 position, glm::vec3 rotation)
	:m_color(color), m_pShader(shader), m_pModel(model)
{
	//set container model
	switch (modelNum) {
	case model1:
		m_size = glm::vec3(12.2f, 3.45f, 5.0f);
		break;
	case model2:
		break;
	case model3:
		break;
	case model4:
		break;
	}

	//create bullet object
	btCollisionShape* boxShape = new btBoxShape(btVector3(m_size.x / 2.0f, m_size.y/2.0f, m_size.z/2.0f));

	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(0.f);

	btVector3 localInertia(0, 0, 0);

	boxShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(btVector3(position.x, position.y, position.z));

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);
	m_pBulletBody = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(m_pBulletBody);
	
	//set rotation
	btTransform transform;
	m_pBulletBody->getMotionState()->getWorldTransform(transform);
	btQuaternion quat;
	quat.setEuler(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
	transform.setRotation(quat);
	m_pBulletBody->getMotionState()->setWorldTransform(transform);
	m_pBulletBody->setCenterOfMassTransform(transform);

	//load textures
	t_albedo = TowerDelivery::loadTexture("assets/textures/container/albedo.png");

	t_normal = TowerDelivery::loadTexture("assets/textures/container/normal.png");
	t_metallic = TowerDelivery::loadTexture("assets/textures/container/metallic.png");
	t_roughness = TowerDelivery::loadTexture("assets/textures/container/roughness.png");
	t_ao = TowerDelivery::loadTexture("assets/textures/container/ao.png");

}

void ContainerObject::Draw()
{
	m_pShader->Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t_albedo);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t_normal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, t_metallic);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, t_roughness);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, t_ao);

	glm::mat4 model = glm::mat4(1.0f);
	btTransform trans;
	btScalar btModelMatrix[16];

	m_pBulletBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(btModelMatrix);

	model = btScalar2mat4(btModelMatrix);


	m_pShader->setMat4("model", model);

	m_pModel->Draw(*m_pShader);
}
