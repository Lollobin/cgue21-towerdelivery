#pragma once

#include "Core.h"

namespace TowerDelivery {

	class TOWERDELIVERY_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
	};

	//To be defined in Client
	Application* CreateApplication();

}