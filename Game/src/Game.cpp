#include "tdpch.h"
#include <TowerDelivery.h>

class ExampleLayer : public TowerDelivery::Layer {
public:
	ExampleLayer()
		:Layer("Example")
	{
	}

	void OnUpdate() override {
		TD_INFO("ExampleLayer::Update");
	}

	void OnEvent(TowerDelivery::Event& event) override {
		TD_TRACE("{0}", event);
	}
};

class Game : public TowerDelivery::Application {
public:
	Game() {
		//PushLayer(new ExampleLayer());
	}

	~Game() {}
};

TowerDelivery::Application* TowerDelivery::CreateApplication() {
	return new Game();
}