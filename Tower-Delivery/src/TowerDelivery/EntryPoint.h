#pragma once

#ifdef TD_PLATFORM_WINDOWS

extern TowerDelivery::Application* TowerDelivery::CreateApplication();

int main(int argc, char** argv)
{
	auto app = TowerDelivery::CreateApplication();
	app->Run();
	delete app;
}

#endif
