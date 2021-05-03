#include <TowerDelivery.h>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

class ExampleLayer : public TowerDelivery::Layer {
public:
	ExampleLayer()
		:Layer("Example"),
		useDebugCamera(false)
	{
		//setup bullet world
		btDefaultCollisionConfiguration* collision_configuration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collision_configuration);
		btDbvtBroadphase* broadphase = new btDbvtBroadphase();
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
		dynamicsWorld.reset(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration));
		dynamicsWorld->setGravity(btVector3(0, -10, 0));

		//setup rendering context
		glEnable(GL_DEPTH_TEST);
		shader.reset(new TowerDelivery::Shader("assets/shader/vs_main.glsl", "assets/shader/fs_main.glsl"));
		shaderHDR.reset(new TowerDelivery::Shader("assets/shader/vs_hdr.glsl", "assets/shader/fs_hdr.glsl"));
		shaderLight.reset(new TowerDelivery::Shader("assets/shader/vs_main.glsl", "assets/shader/fs_light.glsl"));

		//setup character
		characterController = new TowerDelivery::CharacterController(0.5f, 0.5f, 60.0f, btVector3(0.0f, 3.0f, 0.0f), dynamicsWorld.get());
		characterModel = new TowerDelivery::Model("assets/models/character/character.obj");

		//setup cameras
		playerCamera = new TowerDelivery::PlayerCamera(characterController);
		camera = new TowerDelivery::Camera(glm::vec3(0.0f, 1.0f, 4.0f));
		projectionMatrix = glm::perspective(glm::radians(45.0f), window_width / window_height, 0.1f, 100.0f);

		//setup model for lighting
		lightModel = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(1.0f, 1.0f, 1.0f));

		//load model of tower
		stbi_set_flip_vertically_on_load(true);
		ourModel = new TowerDelivery::Model("assets/models/tower/tower1.obj");

		//load textures
		//tex_container = loadTexture("assets/textures/container_diffuse.png");
		tex_pavement = loadTexture("assets/textures/pavement_diffuse.png");

		tex_diff_container = loadTexture("assets/textures/container_diffuse.png");
		tex_spec_container = loadTexture("assets/textures/container_specular.png");

		//create floor
		{
			btCollisionShape* groundShape = new btBoxShape(btVector3(20.0f, 0.5f, 20.0f));

			collisionShapes.push_back(groundShape);

			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(0.f);

			bool isDynamic = false;

			btVector3 localInertia(0, 0, 0);

			groundShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(0.0f, -0.5f, 0.0f));

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
			btRigidBody* bt_floor = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(bt_floor);

			TowerDelivery::VertexArray* gl_floor = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(40.0f, 1.0f, 40.0f));
			TowerDelivery::GameObject* m_floor = new TowerDelivery::GameObject(gl_floor, bt_floor, &tex_pavement);
			m_gameObjects.push_back(m_floor);
		}

		//create static cube
		{
			btCollisionShape* boxShape = new btBoxShape(btVector3(2.5f, 2.5f, 2.5f));

			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(0.f);

			bool isDynamic = false;

			btVector3 localInertia(0, 0, 0);

			boxShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(10.5f, 2.5f, -10.5f));

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);
			btRigidBody* bt_staticCube = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(bt_staticCube);

			TowerDelivery::VertexArray* gl_staticCube = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(5.0f, 5.0f, 5.0f));
			TowerDelivery::GameObject* m_staticCube = new TowerDelivery::GameObject(gl_staticCube, bt_staticCube, &tex_pavement);
			m_gameObjects.push_back(m_staticCube);
		}

		//create dynamic cube
		{
			btCollisionShape* boxShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));

			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(100.f);

			bool isDynamic = true;

			btVector3 localInertia(0, 0, 0);

			boxShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(0, 5, -4));

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);
			btRigidBody* bt_dynamicCube = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(bt_dynamicCube);

			TowerDelivery::VertexArray* gl_dynamicCube = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(2.0f, 2.0f, 2.0f));
			TowerDelivery::GameObject* m_dynamicCube = new TowerDelivery::GameObject(gl_dynamicCube, bt_dynamicCube, &tex_diff_container, &tex_spec_container);
			m_gameObjects.push_back(m_dynamicCube);
		}

		//create platform
		{
			btCollisionShape* boxShape = new btBoxShape(btVector3(2.5f, 0.1f, 2.5f));

			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(0.f);

			bool isDynamic = false;

			btVector3 localInertia(0, 0, 0);

			boxShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(5.5f, 7.0f, -17.5f));

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);
			btRigidBody* bt_platform = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(bt_platform);

			TowerDelivery::VertexArray* gl_platform = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(5.0f, 0.2f, 5.0f));
			TowerDelivery::GameObject* m_platform = new TowerDelivery::GameObject(gl_platform, bt_platform, &tex_pavement);
			m_gameObjects.push_back(m_platform);
		}

		//setup floating point framebuffer for hdr shader
		glGenFramebuffers(1, &hdrFBO);
		// create floating point color buffer
		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei)window_width, (GLsizei)window_height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// create depth buffer (renderbuffer)

		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)window_width, (GLsizei)window_height);
		// attach buffers
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//shader configuration
		shader->Bind();
		shader->setInt("material.diffuse", 0);
		shader->setInt("material.specular", 1);
		shaderHDR->Bind();
		shaderHDR->setInt("hdrBuffer", 0);
	}

	void OnUpdate(TowerDelivery::Timestep ts) override {
		//update world and character
		dynamicsWorld->stepSimulation(1.f / 60.f);
		characterController->OnUpdate(ts);

		//prepare for rendering
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. render scene into floating point framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Bind();
		shader->setMat4("projection", projectionMatrix);

		shaderLight->Bind();
		shaderLight->setMat4("projection", projectionMatrix);

		if (useDebugCamera) {
			shader->Bind();
			shader->setVec3("viewPos", camera->Position);
			shader->setMat4("view", camera->GetViewMatrix());

			shaderLight->Bind();
			shaderLight->setMat4("view", camera->GetViewMatrix());
		}
		else {
			shader->Bind();
			shader->setVec3("viewPos", playerCamera->GetPosition());
			shader->setMat4("view", playerCamera->GetViewMatrix());

			shaderLight->Bind();
			shaderLight->setMat4("view", playerCamera->GetViewMatrix());
		}

		shader->Bind();

		//set directional lighting
		shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shader->setVec3("dirLight.diffuse", 0.1f, 0.1f, 0.1f);
		shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		//set point lights

		shader->setVec3("pointLights[0].position", 0.0f, 3.0f, 0.0f);
		shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		shader->setVec3("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
		shader->setVec3("pointLights[0].specular", 2.0f, 2.0f, 2.0f);
		shader->setFloat("pointLights[0].constant", 1.0f);
		shader->setFloat("pointLights[0].linear", 0.09f);
		shader->setFloat("pointLights[0].quadratic", 0.032f);

		shader->setVec3("pointLights[1].position", -6.5f, 2.0f, -10.0f);
		shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		shader->setVec3("pointLights[1].diffuse", 10.0f, 10.0f, 10.0f);
		shader->setVec3("pointLights[1].specular", 2.0f, 2.0f, 2.0f);
		shader->setFloat("pointLights[1].constant", 1.0f);
		shader->setFloat("pointLights[1].linear", 0.09f);
		shader->setFloat("pointLights[1].quadratic", 0.032f);

		/*
		//set spotlight

		shader->setVec3("Spotlight.position", 0.0f, 3.0f, 0.0f);
		shader->setVec3("Spotlight.direction", 0.0f, -1.0f, 0.0f);

		shader->setFloat("Spotlight.cutOff", 30.0f);
		shader->setFloat("Spotlight.outerCutOff", 50.0f);

		shader->setFloat("Spotlight.constant", 1.0f);
		shader->setFloat("Spotlight.linear", 0.09);
		shader->setFloat("Spotlight.quadratic", 0.032);
		*/

		//set shininess for all models
		shader->setFloat("material.shininess", 1.0f);

		//prepare drawing objects
		glm::mat4 model = glm::mat4(1.0f);

		//draw character
		shader->setMat4("model", characterController->GetModelMatrix());
		characterModel->Draw(*shader);

		//draw tower
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, -10.0f));
		shader->setMat4("model", model);
		ourModel->Draw(*shader);

		//draw game objects
		for each (TowerDelivery::GameObject * gameObject in m_gameObjects)
		{
			gameObject->OnUpdate();
			gameObject->Draw(shader.get());
		}

		//draw point light as cube
		shaderLight->Bind();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.5f, 2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		shaderLight->setMat4("model", model);
		shaderLight->setVec3("lightColor", glm::vec3(10.0f, 10.0f, 10.0f));
		lightModel->draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderHDR->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		shaderHDR->setInt("hdr", true);
		shaderHDR->setFloat("exposure", 1.0f);
		renderQuad();

		if (useDebugCamera)
			camera->OnUpdate(ts);
		else
			playerCamera->OnUpdate(ts);
	}

	void OnEvent(TowerDelivery::Event& event) override {
		//TD_TRACE("{0}", event);
		TowerDelivery::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<TowerDelivery::KeyPressedEvent>(TD_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(TowerDelivery::KeyPressedEvent& event) {
		if (event.GetKeyCode() == TD_KEY_F2) {
			useDebugCamera = !useDebugCamera;
		}
		return true;
	}

	void renderQuad()
	{
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	unsigned int loadTexture(char const* path)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

private:
	//settings
	bool useDebugCamera;
	bool useHDR;
	float window_width = 1280.0f;
	float window_height = 768.0f;

	//rendering
	TowerDelivery::Camera* camera;
	TowerDelivery::PlayerCamera* playerCamera;

	glm::mat4 projectionMatrix;

	std::shared_ptr<TowerDelivery::Shader> shader;
	std::shared_ptr<TowerDelivery::Shader> shaderHDR;
	std::shared_ptr<TowerDelivery::Shader> shaderLight;

	unsigned int hdrFBO;
	unsigned int colorBuffer;
	unsigned int rboDepth;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

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
	unsigned int tex_pavement;

	//game objects
	std::vector<TowerDelivery::GameObject*> m_gameObjects;
};

class Game : public TowerDelivery::Application {
public:
	Game() {
		PushLayer(new ExampleLayer());
	}

	~Game() {}
};

TowerDelivery::Application* TowerDelivery::CreateApplication() {
	return new Game();
}