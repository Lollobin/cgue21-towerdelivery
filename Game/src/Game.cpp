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
		shader.reset(new TowerDelivery::Shader("assets/shader/vertex.glsl", "assets/shader/fragment.glsl"));
		hdrShader.reset(new TowerDelivery::Shader("assets/shader/hdr-vertex.glsl", "assets/shader/hdr-fragment.glsl"));
		

		//setup character
		characterController = new TowerDelivery::CharacterController(0.5f, 0.5f, 60.0f, btVector3(0.0f, 3.0f, 0.0f), dynamicsWorld.get());
		characterModel = new TowerDelivery::Model("assets/models/character/character.obj");

		//setup cameras
		playerCamera = new TowerDelivery::PlayerCamera(characterController);
		camera = new TowerDelivery::Camera(glm::vec3(0.0f, 1.0f, 4.0f));
		projectionMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 768.0f, 0.1f, 100.0f);

		//load model of tower
		stbi_set_flip_vertically_on_load(true);
		ourModel = new TowerDelivery::Model("assets/models/tower/tower1.obj");


		//create collision shape for Floor
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
			bt_floor = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(bt_floor);
		}

		//create model for floor

		gl_floor = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(40.0f, 1.0f, 40.0f));



		//create collision shape for static cube
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
			bt_staticCube = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(bt_staticCube);
		}

		gl_staticCube = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(5.0f, 5.0f, 5.0f));

		//create collision shape for dynamic cube
		{
			btCollisionShape* boxShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));

			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(50.f);

			bool isDynamic = true;

			btVector3 localInertia(0, 0, 0);

			boxShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(0, 5, -4));

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);
			bt_dynamicCube = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(bt_dynamicCube);
		}

		gl_dynamicCube = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(2.0f, 2.0f, 2.0f));

		//create collision shape for platform
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
			bt_platform = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(bt_platform);
		}

		gl_platform = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(5.0f, 0.2f, 5.0f));

		
		
		//load textures
		tex_container = loadTexture("assets/textures/container_diffuse.png");
		tex_pavement = loadTexture("assets/textures/pavement_diffuse.png");


		//setup floating point framebuffer for hdr shader
		glGenFramebuffers(1, &hdrFBO);
		// create floating point color buffer
		
		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280.0f, 768.0f, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// create depth buffer (renderbuffer)
		
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280.0f, 768.0f);
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
		hdrShader->Bind();
		hdrShader->setInt("hdrBuffer", 0);
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
		if (useDebugCamera) {
			shader->setVec3("viewPos", camera->Position);
			shader->setMat4("view", camera->GetViewMatrix());
		}
		else {
			shader->setVec3("viewPos", playerCamera->GetPosition());
			shader->setMat4("view", playerCamera->GetViewMatrix());
		}

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
		shader->setFloat("pointLights[0].linear", 0.09);
		shader->setFloat("pointLights[0].quadratic", 0.032);
		

		shader->setVec3("pointLights[1].position", -6.5f, 2.0f, -10.0f);
		shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		shader->setVec3("pointLights[1].diffuse", 10.0f, 10.0f, 10.0f);
		shader->setVec3("pointLights[1].specular", 2.0f, 2.0f, 2.0f);
		shader->setFloat("pointLights[1].constant", 1.0f);
		shader->setFloat("pointLights[1].linear", 0.09);
		shader->setFloat("pointLights[1].quadratic", 0.032);

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
		shader->setFloat("material.shininess", 5.0f);

		//prepare drawing objects
		btCollisionObject* obj;
		btRigidBody* body;
		btTransform trans;
		btScalar btModelMatrix[16];
		glm::mat4 model = glm::mat4(1.0f);

		//draw character
		shader->setMat4("model", characterController->GetModelMatrix());
		characterModel->Draw(*shader);

		//draw tower
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, -10.0f));
		shader->setMat4("model", model);
		ourModel->Draw(*shader);

		//draw dynamic cube
		glBindTexture(GL_TEXTURE_2D, tex_container);
		model = getRigidBodyModelMatrix(bt_dynamicCube);
		shader->setMat4("model", model);
		gl_dynamicCube->draw();

		//draw static cube
		glBindTexture(GL_TEXTURE_2D, tex_pavement);
		model = getRigidBodyModelMatrix(bt_staticCube);
		shader->setMat4("model", model);
		gl_staticCube->draw();

		//draw platform
		model = getRigidBodyModelMatrix(bt_platform);
		shader->setMat4("model", model);
		gl_platform->draw();

		//draw floor
		model = getRigidBodyModelMatrix(bt_floor);
		shader->setMat4("model", model);
		gl_floor->draw();

		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		hdrShader->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		hdrShader->setInt("hdr", true);
		hdrShader->setFloat("exposure", 1.0f);
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
	//rendering
	TowerDelivery::Camera* camera;
	TowerDelivery::PlayerCamera* playerCamera;
	bool useDebugCamera;
	glm::mat4 projectionMatrix;
	std::shared_ptr<TowerDelivery::Shader> shader;

	std::shared_ptr<TowerDelivery::Shader> hdrShader;
	unsigned int hdrFBO;
	unsigned int colorBuffer;
	unsigned int rboDepth;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	//character
	TowerDelivery::CharacterController* characterController;
	TowerDelivery::Model* characterModel;

	//opengl models
	std::shared_ptr<TowerDelivery::VertexArray> objectModel;
	vector<TowerDelivery::VertexArray> vertexArrays;
	TowerDelivery::Model* ourModel;

	//bullet
	std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	//temporary bullet and opengl objects
	btRigidBody* bt_floor;
	btRigidBody* bt_dynamicCube;
	btRigidBody* bt_staticCube;
	btRigidBody* bt_platform;

	TowerDelivery::VertexArray* gl_floor;
	TowerDelivery::VertexArray* gl_dynamicCube;
	TowerDelivery::VertexArray* gl_staticCube;
	TowerDelivery::VertexArray* gl_platform;

	unsigned int tex_container;
	unsigned int tex_pavement;

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