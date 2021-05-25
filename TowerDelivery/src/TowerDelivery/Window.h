#pragma once

#include "tdpch.h"

#include "TowerDelivery/Core/Core.h"
#include "TowerDelivery/Events/Event.h"

namespace TowerDelivery {

	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		
		WindowProps(const std::string& title = "TowerDelivery Engine",
					unsigned int width = 1280,
					unsigned int height = 768)
					: Title(title), Width(width), Height(height) {
		}
		
	};

	//just an Interface
	class TOWERDELIVERY_API Window {
	public: 
		using EventCallbackFn = std::function<void(Event&)>;

		virtual  ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual unsigned int* GetScreenSize() = 0;
		
		virtual void SetSize(unsigned int width, unsigned int height) = 0;
		virtual void SetFullscreen(bool fullscreen) = 0;
		virtual bool IsFullscreen() = 0;

		

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetRefreshRate(unsigned int rate) const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}