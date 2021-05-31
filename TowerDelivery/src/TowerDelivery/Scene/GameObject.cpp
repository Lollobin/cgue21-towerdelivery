#include "tdpch.h"
#include "GameObject.h"

namespace TowerDelivery {
	GameObject::GameObject(VertexArray* vertexArray, unsigned int* diffuseTex, btRigidBody* bulletBody, unsigned int* specularTex)
		: m_pVertexArray(vertexArray), m_pBulletBody(bulletBody), m_modelMatrix(glm::mat4(1.0f)),
		m_pTextureDiffuse(diffuseTex), m_pTextureSpecular(specularTex)
	{
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::OnUpdate()
	{
		//update model matrix
		if (m_pBulletBody) {
			m_modelMatrix = getRigidBodyModelMatrix(m_pBulletBody);
		}
	}

	void GameObject::Draw(Shader* shader)
	{
		//bind diffuse texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *m_pTextureDiffuse);

		//bind specular texture (if defined)
		glActiveTexture(GL_TEXTURE1);
		if (m_pTextureSpecular)
			glBindTexture(GL_TEXTURE_2D, *m_pTextureSpecular);
		else
			glBindTexture(GL_TEXTURE_2D, 0);

		//set model matrix
		shader->setMat4("model", m_modelMatrix);
		m_pVertexArray->draw();

		//unbind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void GameObject::SetModelMatrix(glm::mat4 modelMatrix)
	{
		m_modelMatrix = modelMatrix;
	}
}