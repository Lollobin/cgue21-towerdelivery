#pragma once

#include "TowerDelivery.h"
#include "ContainerObject.h"

class MainLayer : public TowerDelivery::Layer {

public:
	MainLayer(TowerDelivery::Application* game);
	virtual ~MainLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(TowerDelivery::Timestep ts) override;
	void OnEvent(TowerDelivery::Event& e) override;

private:

	bool OnKeyPressedEvent(TowerDelivery::KeyPressedEvent& event);
	bool OnWindowResizeEvent(TowerDelivery::WindowResizeEvent& event);

	void renderQuad();

	inline btVector3 glm2bt(const glm::vec3& vec)
	{
		return { vec.x, vec.y, vec.z };
	}

private:
	//settings
	unsigned int window_width;
	unsigned int window_height;
	unsigned int refresh_rate;
	bool fullscreen;

	float exposure;

	bool useDebugCamera;
	bool bloom = true;

	//rendering
	TowerDelivery::Camera* camera;
	TowerDelivery::PlayerCamera* playerCamera;
	float didJustCollide = false;

	glm::mat4 projectionMatrix;

	std::shared_ptr<TowerDelivery::Shader> shader;
	std::shared_ptr<TowerDelivery::Shader> shaderLight;
	std::shared_ptr<TowerDelivery::Shader> shaderBlur;
	std::shared_ptr<TowerDelivery::Shader> shaderFinal;
	std::shared_ptr<TowerDelivery::Shader> shaderParticle;
	std::shared_ptr<TowerDelivery::Shader> shaderText;

	std::shared_ptr<TowerDelivery::Shader> shaderPBR;

	unsigned int hdrFBO;
	unsigned int colorBuffers[2];
	unsigned int rboDepth;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];

	TowerDelivery::Application* m_Game;

	//character
	TowerDelivery::CharacterController* characterController;
	TowerDelivery::Model* characterModel;

	//opengl models
	TowerDelivery::Model* ourModel;
	TowerDelivery::VertexArray* lightModel;
	TowerDelivery::VertexArray* cubeModel;

	//bullet
	std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	//textures
	unsigned int tex_diff_container;
	unsigned int tex_spec_container;
	unsigned int tex_diff_pavement;

	unsigned int albedo;
	unsigned int normal;
	unsigned int metallic;
	unsigned int roughness;
	unsigned int ao;

	unsigned int tex_particle;

	//game objects
	std::vector<TowerDelivery::GameObject*> m_gameObjects;

	//detection areas
	TowerDelivery::DetectionArea* loseArea;
	bool lost = false;

	//particle systems
	TowerDelivery::ParticleSystem* particleSystem;

	//game logic
	unsigned int lives = 3;
	unsigned int packages = 4;
	unsigned int packagesCollected = 0;


	glm::mat4 cp_models[4];
	bool cp_reached[4];
	TowerDelivery::DetectionArea* cp_areas[4];
	glm::vec3 cp_spawnPos[4];

	//containers
	ContainerObject* testContainer;

	TowerDelivery::Model* container1;
	TowerDelivery::Model* container2;
	TowerDelivery::Model* container3;
	TowerDelivery::Model* container4;

	//text renderer
	TowerDelivery::TextRenderer* text;

};