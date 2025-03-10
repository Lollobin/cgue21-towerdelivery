#include "tdpch.h"
#include "Application.h"

#include "TowerDelivery/Core/Log.h"

#include "Input.h"
#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include "assimp/Importer.hpp"
#include "TowerDelivery/Rendering/Mesh.h"
#include "TowerDelivery/Events/KeyEvent.h"
#include "TowerDelivery/Core/KeyCodes.h"

namespace TowerDelivery {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		TD_CORE_ASSERT(!s_Instance, "Applicaton already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		while (m_Running) {

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_Window->OnUpdate();

			if (Input::IsKeyPressed(TD_KEY_ESCAPE)) {
				m_Running = false;
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());

		return false;
	}
}