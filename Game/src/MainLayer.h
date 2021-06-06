#pragma once

#include "TowerDelivery.h"

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

	//could be moved to another class
	void renderQuad();
	unsigned int loadTexture(char const* path);

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

	glm::mat4 projectionMatrix;

	std::shared_ptr<TowerDelivery::Shader> shader;
	std::shared_ptr<TowerDelivery::Shader> shaderLight;
	std::shared_ptr<TowerDelivery::Shader> shaderBlur;
	std::shared_ptr<TowerDelivery::Shader> shaderFinal;
	std::shared_ptr<TowerDelivery::Shader> shaderText;

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

	//bullet
	std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	//textures
	unsigned int tex_diff_container;
	unsigned int tex_spec_container;
	unsigned int tex_diff_pavement;

	//game objects
	std::vector<TowerDelivery::GameObject*> m_gameObjects;

	//detection areas
	TowerDelivery::DetectionArea* loseArea;
	bool lost = false;
};