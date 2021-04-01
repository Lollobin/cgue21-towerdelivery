#include "tdpch.h"
#include "Application.h"

#include "TowerDelivery/Events/ApplicationEvent.h"
#include "TowerDelivery/Events/MouseEvent.h"
#include "TowerDelivery/Events/KeyEvent.h"
#include "TowerDelivery/Log.h"

namespace TowerDelivery {

	Application::Application() {}
	Application::~Application() {}

	void Application::Run() {
		
		//test
		WindowResizeEvent e(13, 15);
		TD_TRACE(e);

		while (true);
	}
}
