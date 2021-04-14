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

		float w = 0.3f;
		float h = 0.3f;
		float d = 0.3f;

		//generate vertices
		std::vector<float> positions({
		 w,h,d,        // 0 (1,0,0) --> 0
		w,h,d,       // 0 (0,1,0) --> 1
		w,h,d,       // 0 (0,0,1) --> 2

		w,h,-d,       // 1 (1,0,0) --> 3
		w,h,-d,       // 1 (0,1,0) --> 4
		w,h,-d,       // 1 (0,0,-1) --> 5

		w,-h,d,       // 2 (1,0,0) --> 6
		w,-h,d,       // 2 (0,-1,0)--> 7
		w,-h,d,       // 2 (0,0,1) --> 8

		w,-h,-d,       // 3 (1,0,0) --> 9
		w,-h,-d,       // 3 (0,-1,0) --> 10
		w,-h,-d,       // 3 (0,0,-1) --> 11

		-w,h,d,       // 4 (-1,0,0) -->12
		-w,h,d,       // 4 (0,1,0) --> 13
		-w,h,d,       // 4 (0,0,1) --> 14

		-w,h,-d,       // 5 (-1,0,0) --> 15
		-w,h,-d,       //5 (0,1,0) --> 16
		-w,h,-d,       // 5 (0,0-1) --> 17

		-w,-h,d,       // 6 (-1,0,0) --> 18
		-w,-h,d,       //6 (0,-1,0) --> 19
		-w,-h,d,       // 6 (0,0,1) --> 20

		-w,-h,-d,       // 7 (-1,0,0) --> 21
		-w,-h,-d,       // 7 (0,-1,0) --> 22
		-w,-h,-d, });
		//generate indices
		std::vector<unsigned int> indices({
		   6,3,0, //(1,0,0)

		   6,9,3, //(1,0,0)

		14,8,2, //(0,0,1)

		14,20,8, //(0,0,1)

		19,22,7, //(0,-1,0)

		7,22,10, //(0,-1,0)

		18,12,21, //(-1,0,0)

		12,15,21, //(-1,0,0)

		13,1,16, //(0,1,0)

		1,4,16, //(0,1,0)

		5,11,17, //(0,0,-1)

		23,17,11, //(0,0,-1) 
			});
		//generate normals
		std::vector<float> normals({
		1,0,0,
		0,1,0,
		0,0,1,

		1,0,0,
		0,1,0,
		0,0,-1,

		1,0,0,
		0,-1,0,
		0,0,1,

		1,0,0,
		0,-1,0,
		0,0,-1,

		-1,0,0,
		0,1,0,
		0,0,1,

		-1,0,0,
		0,1,0,
		0,0,-1,

		-1,0,0,
		0,-1,0,
		0,0,1,

		-1,0,0,
		0,-1,0,
		0,0,-1
			});

		cube.reset(new TowerDelivery::VertexArray(positions, indices, normals));

		shader.reset(new TowerDelivery::Shader("assets/shader/vertex.glsl", "assets/shader/fragment.glsl"));
		
		shader->Bind();	
	}
	  
	void OnUpdate() override {
		camera.OnUpdate();

		glClearColor(0.2f, 0.2f, 0.2f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 vpMatrix = projectionMatrix * camera.GetViewMatrix();

		GLint location = glGetUniformLocation(shader->m_RendererID, "vpMatrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, &vpMatrix[0][0]);

		glm::vec3 color = glm::vec3(0.8, 0.1, 0.2);
		location = glGetUniformLocation(shader->m_RendererID, "color");
		glUniform3fv(location, 1, &color[0]);

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