#define STB_IMAGE_IMPLEMENTATION
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
		shader.reset(new TowerDelivery::Shader("assets/shader/vertex.glsl", "assets/shader/fragment.glsl"));
		glEnable(GL_DEPTH_TEST);

		//setup character
		characterController = new TowerDelivery::CharacterController(0.5f, 0.5f, 60.0f, btVector3(0.0f, 3.0f, 0.0f), dynamicsWorld.get());
		characterModel = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(1.0f, 1.0f, 1.0f));

		//setup cameras
		playerCamera = new TowerDelivery::PlayerCamera(characterController);
		camera = Camera(glm::vec3(0.0f, 1.0f, 4.0f));
		projectionMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

		//create collision shape for Floor
		{
			btCollisionShape* groundShape = new btBoxShape(btVector3(50, 1, 50));

			collisionShapes.push_back(groundShape);

			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(0.f);

			bool isDynamic = false;

			btVector3 localInertia(0, 0, 0);

			groundShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(0, -1, 0));

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(body);
		}

		//create model for floor
		objectModel.reset(new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(100.0f, 1.0f, 100.0f)));

		//TowerDelivery::VertexArray floor(TowerDelivery::VertexArray::createCubeVertexArray(100.0f, 1.0f, 100.0f));
		vertexArrays.push_back(*objectModel);

		//create collision shape for box 1
		{
			btCollisionShape* boxShape = new btBoxShape(btVector3(2.0f, 2.0f, 2.0f));

			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(0.f);

			bool isDynamic = false;

			btVector3 localInertia(0, 0, 0);

			boxShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(10, 2, 5));

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(body);
		}

		TowerDelivery::VertexArray cube1(TowerDelivery::VertexArray::createCubeVertexArray(4.0f, 4.0f, 4.0f));
		//ObjectModel.reset(new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(4.0f, 4.0f, 4.0f)));
		//vertexArrays.push_back(cube1);

		//bind textures
		std::string path("assets/textures/container2.png");
		unsigned int diffuseTex = loadTexture(path.c_str());
		shader->setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTex);

		//stbi_set_flip_vertically_on_load(true);
		//ourModel = new TowerDelivery::Model("assets/models/backpack/backpack.obj");
		//ourModel = new TowerDelivery::Model("C:/dev/tower-delivery/Game/assets/models/tower1.obj");
	}

	void OnUpdate(TowerDelivery::Timestep ts) override {
		//update world and character
		dynamicsWorld->stepSimulation(1.f / 60.f);
		characterController->OnUpdate(ts);

		//prepare for rendering
		glClearColor(0.2f, 0.2f, 0.2f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader->Bind();
		shader->setMat4("projection", projectionMatrix);
		if (useDebugCamera) {
			shader->setVec3("viewPos", camera.Position);
			shader->setMat4("view", camera.GetViewMatrix());
		}
		else {
			shader->setVec3("viewPos", playerCamera->GetPosition());
			shader->setMat4("view", playerCamera->GetViewMatrix());
		}

		//set directional lighting
		shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		//set point lights
		shader->setVec3("pointLights[0].position", 0.0f, 3.0f, 0.0f);
		shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		shader->setFloat("pointLights[0].constant", 1.0f);
		shader->setFloat("pointLights[0].linear", 0.09);
		shader->setFloat("pointLights[0].quadratic", 0.032);

		//set shininess for all models
		shader->setFloat("material.shininess", 5.0f);

		//ourModel->Draw(*shader);

		//prepare drawing objects
		btCollisionObject* obj;
		btRigidBody* body;
		btTransform trans;
		btScalar btModelMatrix[16];
		glm::mat4 model = glm::mat4(1.0f);

		//draw character
		obj = dynamicsWorld->getCollisionObjectArray()[0];
		body = btRigidBody::upcast(obj);
		body->getMotionState()->getWorldTransform(trans);
		trans.getOpenGLMatrix(btModelMatrix);
		model = btScalar2mat4(btModelMatrix);

		model = glm::rotate(model, glm::radians(characterController->GetRotation()), glm::vec3(0.0f, -1.0f, 0.0f));

		shader->setMat4("model", model);
		characterModel->draw();

		for (int i = 0; i < vertexArrays.size(); i++) {
			obj = dynamicsWorld->getCollisionObjectArray()[i + 1];
			body = btRigidBody::upcast(obj);

			if (body && body->getMotionState()) {
				body->getMotionState()->getWorldTransform(trans);
			}
			else {
				trans = obj->getWorldTransform();
			}

			trans.getOpenGLMatrix(btModelMatrix);
			model = btScalar2mat4(btModelMatrix);

			shader->setMat4("model", model);

			vertexArrays[i].draw();
		}

		if (useDebugCamera)
			camera.OnUpdate(ts);
		else
			playerCamera->OnUpdate(ts);
	}

	void OnEvent(TowerDelivery::Event& event) override {
		//TD_TRACE("{0}", event);
		TowerDelivery::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<TowerDelivery::KeyPressedEvent>(TD_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	glm::mat4 btScalar2mat4(btScalar* matrix) {
		return glm::mat4(
			matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[12], matrix[13], matrix[14], matrix[15]);
	}

	bool OnKeyPressedEvent(TowerDelivery::KeyPressedEvent& event) {
		if (event.GetKeyCode() == TD_KEY_F2) {
			useDebugCamera = !useDebugCamera;
		}
		return true;
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
	Camera camera;
	TowerDelivery::PlayerCamera* playerCamera;
	bool useDebugCamera;
	glm::mat4 projectionMatrix;
	std::shared_ptr<TowerDelivery::Shader> shader;

	//character
	TowerDelivery::CharacterController* characterController;
	TowerDelivery::VertexArray* characterModel;

	//opengl models
	std::shared_ptr<TowerDelivery::VertexArray> objectModel;
	vector<TowerDelivery::VertexArray> vertexArrays;
	//list<TowerDelivery::VertexArray> vertexArrays;

	//bullet
	std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
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