#include "tdpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include "glad/glad.h"

#include "glm/glm.hpp"

#include "Input.h"

#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"
#include "Rendering/Camera.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace TowerDelivery {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;


	Application* Application::s_Instance = nullptr;

	Application::Application() {
		TD_CORE_ASSERT(!s_Instance, "Applicaton already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		/*
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	float vertices[3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	m_VertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	uint32_t indices[3] = { 0,1,2 };
	m_IndexBuffer.reset(new IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
	*/
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//TD_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		/*
		btDefaultCollisionConfiguration* collision_configuration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collision_configuration);
		btDbvtBroadphase* broadphase = new btDbvtBroadphase();
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
		btDiscreteDynamicsWorld* dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);
		dynamics_world->setGravity(btVector3(0, -10, 0));
		*/
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

		Shader shader("assets/shader/color.vert", "assets/shader/color.frag");
		VertexArray cube = VertexArray(positions, indices, normals);
		
		while (m_Running) {
			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			shader.Bind();
			cube.draw();
			/*
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
			*/

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			auto [x, y] = Input::GetMousePostition();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}