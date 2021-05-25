#pragma once

#ifdef TD_PLATFORM_WINDOWS

extern TowerDelivery::Application* TowerDelivery::CreateApplication();

int main(int argc, char** argv)
{
	TowerDelivery::Log::Init();

	//Code to test the logger
	TD_CORE_WARN("Initialized Log");
	int a = 5;

	auto app = TowerDelivery::CreateApplication();
	app->Run();
	delete app;
}

#endif
