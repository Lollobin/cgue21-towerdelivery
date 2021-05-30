#pragma once

#include "TowerDelivery/Core/Core.h"
#include "TowerDelivery/Core/Timestep.h"
#include "TowerDelivery/Events/Event.h"

namespace TowerDelivery {

	class TOWERDELIVERY_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer()= default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}



