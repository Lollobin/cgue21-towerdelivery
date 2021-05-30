#include "TowerDelivery.h"
#include "TowerDelivery/Core/EntryPoint.h"

#include "MainLayer.h"

class Game : public TowerDelivery::Application {
public:
	Game() {
		PushLayer(new MainLayer(this));
	}

	~Game() {}
};

TowerDelivery::Application* TowerDelivery::CreateApplication() {
	return new Game();
}