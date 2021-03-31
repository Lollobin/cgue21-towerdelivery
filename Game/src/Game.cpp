#include "tdpch.h"
#include <TowerDelivery.h>

class Game : public TowerDelivery::Application {
public:
	Game() {}

	~Game() {}
};

TowerDelivery::Application* TowerDelivery::CreateApplication() {
	return new Game();
}