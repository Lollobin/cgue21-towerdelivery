#pragma once

#include "Core.h"

#include "Window.h"
#include "TowerDelivery/LayerStack.h"
#include "TowerDelivery/Events/Event.h"
#include "TowerDelivery/Events/ApplicationEvent.h"


namespace TowerDelivery {

	class TOWERDELIVERY_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() {
			return *m_Window;
		}
		inline static Application& Get() { 
			return *s_Instance; 
		}

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;

	private:
		static Application* s_Instance;
	};

	//To be defined in Client
	Application* CreateApplication();

}