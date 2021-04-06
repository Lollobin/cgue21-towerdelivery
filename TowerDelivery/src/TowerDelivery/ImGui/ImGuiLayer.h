#pragma once

#include "TowerDelivery/Layer.h"

namespace TowerDelivery {

	class TOWERDELIVERY_API ImGuiLayer : public Layer {

	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach();
		virtual void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
		float m_Time = 0.0f;
	};
}