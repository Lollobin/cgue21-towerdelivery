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

	void GenerateDynamicCubes();

private:
	//settings
	unsigned int window_width;
	unsigned int window_height;
	unsigned int refresh_rate;
	bool fullscreen;
	bool showFPS;

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
	//unsigned int tex_diff_container;
	//unsigned int tex_spec_container;
	//unsigned int tex_diff_pavement;


	/*
	unsigned int albedo;
	unsigned int normal;
	unsigned int metallic;
	unsigned int roughness;
	unsigned int ao;
	*/


	unsigned int c_albedo_black;
	unsigned int c_albedo_blue;
	unsigned int c_albedo_green;
	unsigned int c_albedo_grey;
	unsigned int c_albedo_orange;
	unsigned int c_albedo_red;
	unsigned int c_albedo_white;
	unsigned int c_normal;
	unsigned int c_metallic;
	unsigned int c_roughness;
	unsigned int c_ao;

	unsigned int tex_particle;

	unsigned int tex_diff_cube;
	unsigned int tex_spec_cube;

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

	glm::vec3 spawnPos = glm::vec3(-2.28f, 8.0f, -5.04f);
	//glm::vec3 spawnPos = glm::vec3(-0.62, 40.0, 8.39);

	//containers
	std::vector<ContainerObject*> m_containers;

	ContainerObject* container1;
	ContainerObject* container2;
	ContainerObject* container3;
	ContainerObject* container4;
	ContainerObject* container5;
	ContainerObject* container6;
	ContainerObject* container7;
	ContainerObject* container8;
	ContainerObject* container9;
	ContainerObject* container10;
	ContainerObject* container11;
	ContainerObject* container12;
	ContainerObject* container13;
	ContainerObject* container14;
	ContainerObject* container15;
	ContainerObject* container16;
	ContainerObject* container17;
	ContainerObject* container18;
	ContainerObject* container19;
	ContainerObject* container20;
	ContainerObject* container21;
	ContainerObject* container22;
	ContainerObject* container23;
	ContainerObject* container24;
	ContainerObject* container25;
	ContainerObject* container26;
	ContainerObject* container27;
	ContainerObject* container28;
	ContainerObject* container29;
	ContainerObject* container30;
	ContainerObject* container31;
	ContainerObject* container32;
	ContainerObject* container33;
	ContainerObject* container34;
	ContainerObject* container35;
	ContainerObject* container36;
	ContainerObject* container37;
	ContainerObject* container38;
	ContainerObject* container39;
	ContainerObject* container40;
	ContainerObject* container41;
	ContainerObject* container42;
	ContainerObject* container43;
	ContainerObject* container44;
	

	TowerDelivery::Model* containerType1;
	TowerDelivery::Model* containerType2;
	TowerDelivery::Model* containerType3;
	TowerDelivery::Model* containerType4;

	//text renderer
	TowerDelivery::TextRenderer* text;

	std::vector<glm::vec3> dc_positions;
	std::vector<glm::vec3> dc_sizes;
	std::vector<btRigidBody*> dc_bodies;
	std::vector<TowerDelivery::GameObject*> dc_gameObjects;

	std::vector<glm::vec3> light_positions;
	std::vector<glm::vec3> light_colors;


};