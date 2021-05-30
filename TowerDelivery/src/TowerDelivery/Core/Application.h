#pragma once

#include "TowerDelivery/Core/Core.h"

#include "Window.h"
#include "TowerDelivery/Core/LayerStack.h"
#include "TowerDelivery/Events/Event.h"
#include "TowerDelivery/Events/ApplicationEvent.h"

#include "TowerDelivery/Core/Timestep.h"

#include "TowerDelivery/Rendering/Buffer.h"

namespace TowerDelivery {
	class TOWERDELIVERY_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime;

	private:
		static Application* s_Instance;
	};

	//To be defined in Client
	Application* CreateApplication();
}