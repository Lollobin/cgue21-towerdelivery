#include "tdpch.h"
#include "Application.h"

#include "TowerDelivery/Events/ApplicationEvent.h"
#include "TowerDelivery/Log.h"

namespace TowerDelivery {

	Application::Application() {}
	Application::~Application() {}

	void Application::Run() {
		
		//test
		WindowResizeEvent e(1280, 720);
		TD_TRACE(e);
		
		while (true);
	}
}
