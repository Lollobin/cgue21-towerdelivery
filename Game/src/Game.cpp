#include <TowerDelivery.h>

class ExampleLayer : public TowerDelivery::Layer {
public:
	ExampleLayer()
		:Layer("Example")
	{
		// proj-Matrix settings
		const unsigned int SCR_WIDTH = 800;
		const unsigned int SCR_HEIGHT = 600;
		float lastX = SCR_WIDTH / 2.0f;
		float lastY = SCR_HEIGHT / 2.0f;
		bool firstMouse = true;

		camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

		projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		cube.reset(new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(3.0f, 1.0f, 0.5f)));

		shader.reset(new TowerDelivery::Shader("assets/shader/vertex.glsl", "assets/shader/fragment.glsl"));

		shader->Bind();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	void OnUpdate(TowerDelivery::Timestep ts) override {
		camera.OnUpdate(ts);

		glClearColor(0.2f, 0.2f, 0.2f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->setVec3("viewPos", camera.Position);

		shader->setVec3("material.color", glm::vec3(1.0, 0.5, 0.1));
		shader->setFloat("material.diffuse", 1.0f);
		shader->setFloat("material.shininess", 1.0f);
		shader->setFloat("material.specular", 1.0f);

		shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		shader->setMat4("model", glm::mat4(1.0f));
		shader->setMat4("view", camera.GetViewMatrix());
		shader->setMat4("projection", projectionMatrix);

		cube->draw();
	}

	void OnEvent(TowerDelivery::Event& event) override {
		//TD_TRACE("{0}", event);
	}

private:
	Camera camera;
	glm::mat4 projectionMatrix;

	std::shared_ptr<TowerDelivery::Shader> shader;
	std::shared_ptr<TowerDelivery::VertexArray> cube;
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