#define STB_IMAGE_IMPLEMENTATION
#include <TowerDelivery.h>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

class ExampleLayer : public TowerDelivery::Layer {
public:
	ExampleLayer()
		:Layer("Example")
	{
		btDefaultCollisionConfiguration* collision_configuration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collision_configuration);
		btDbvtBroadphase* broadphase = new btDbvtBroadphase();
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
		dynamicsWorld.reset(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration));
		dynamicsWorld->setGravity(btVector3(0, -10, 0));

		/*
		{
			btCollisionShape* colShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));

			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(1.f);

			bool isDynamic = true;

			btVector3 localInertia(0, 0, 0);

			colShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(0, 3, 0));

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
			btCube = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(btCube);
		}
		*/

		character = new TowerDelivery::CharacterController(0.5f, 0.5f, 1.0f, btVector3(0.0f, 3.0f, 0.0f), dynamicsWorld.get());

		{
			btCollisionShape* colShape = new btBoxShape(btVector3(10, 0.5, 10));

			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(0.f);

			bool isDynamic = false;

			btVector3 localInertia(0, 0, 0);

			colShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(0, -1, 0));

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
			btFloor = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(btFloor);
		}

		// projection settings
		const unsigned int SCR_WIDTH = 1280;
		const unsigned int SCR_HEIGHT = 720;

		float lastX = SCR_WIDTH / 2.0f;
		float lastY = SCR_HEIGHT / 2.0f;
		bool firstMouse = true;

		camera = Camera(glm::vec3(0.0f, 1.0f, 4.0f));

		projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		cube.reset(new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(1.0f, 1.0f, 1.0f)));
		floor.reset(new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(20.0f, 1.0f, 20.0f)));

		shader.reset(new TowerDelivery::Shader("assets/shader/vertex.glsl", "assets/shader/fragment.glsl"));
		shader->Bind();

		glEnable(GL_DEPTH_TEST);

		std::string path("assets/textures/container2.png");

		unsigned int diffuseTex = loadTexture(path.c_str());
		
		shader->setInt("material.diffuse", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTex);
		
	}

	void OnUpdate(TowerDelivery::Timestep ts) override {
		//TD_TRACE("{0}", ts);

		dynamicsWorld->stepSimulation(1.f / 60.f);
		character->OnUpdate(ts);

		//TD_TRACE("Cube Position: {0}, {1}, {2}", float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));

		glClearColor(0.2f, 0.2f, 0.2f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->setVec3("viewPos", camera.Position);

		shader->setFloat("material.shininess", 1.0f);

		shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader->setVec3("dirLight.ambient", 1.1f, 1.1f, 1.1f);
		shader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		shader->setMat4("view", camera.GetViewMatrix());
		shader->setMat4("projection", projectionMatrix);

		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[0];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);

		btScalar btModelMatrix[16];
		trans.getOpenGLMatrix(btModelMatrix);
		glm::mat4 model = glm::mat4(1.0f);
		model = btScalar2mat4(btModelMatrix);

		

		shader->setMat4("model", model);
		cube->draw();

		obj = dynamicsWorld->getCollisionObjectArray()[1];
		body = btRigidBody::upcast(obj);

		body->getMotionState()->getWorldTransform(trans);

		trans.getOpenGLMatrix(btModelMatrix);

		model = btScalar2mat4(btModelMatrix);

		shader->setMat4("model", model);
		floor->draw();

		camera.OnUpdate(ts);
	}

	void OnEvent(TowerDelivery::Event& event) override {
		//TD_TRACE("{0}", event);
	}

	glm::mat4 btScalar2mat4(btScalar* matrix) {
		return glm::mat4(
			matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[12], matrix[13], matrix[14], matrix[15]);
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
	Camera camera;
	glm::mat4 projectionMatrix;

	std::shared_ptr<TowerDelivery::Shader> shader;
	std::shared_ptr<TowerDelivery::VertexArray> cube;
	std::shared_ptr<TowerDelivery::VertexArray> floor;

	std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
	btRigidBody* btCube;
	btRigidBody* btFloor;
	TowerDelivery::CharacterController* character;
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