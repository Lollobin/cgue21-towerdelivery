#pragma once

#include "TowerDelivery/Core/Window.h"
#include "TowerDelivery/Rendering/RenderingContext.h"

#include <GLFW/glfw3.h>

namespace TowerDelivery {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;
		inline void SetSize(unsigned int width, unsigned int height) override {
			glfwSetWindowSize(m_Window, width, height);
		}

		inline unsigned int GetWidth() const override {
			return m_Data.Width;
		}
		inline unsigned int GetHeight() const override {
			return m_Data.Height;
		}

		inline unsigned int* GetScreenSize() {
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			unsigned int size[2];
			size[0] = mode->width;
			size[1] = mode->height;
			return size;
		}

		inline bool IsFullscreen() override {
			return glfwGetWindowMonitor(m_Window) != nullptr;
		}

		void SetFullscreen(bool fullscreen) override;

		inline void SetEventCallback(const EventCallbackFn& callback) override {
			m_Data.EventCallback = callback;
		}
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		void SetRefreshRate(unsigned int rate) const override;

		inline virtual void* GetNativeWindow() const {
			return m_Window;
		}

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		RenderingContext* m_Context;

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
			GLFWmonitor* Monitor;
		};

		WindowData m_Data;

		std::array< int, 2 > m_WindowPos{ 0, 0 };
		std::array< int, 2 > m_WindowSize{ 0, 0 };

	};
}