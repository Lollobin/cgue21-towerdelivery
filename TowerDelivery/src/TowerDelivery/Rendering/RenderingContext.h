#pragma once

struct GLFWwindow;

namespace TowerDelivery {

	class RenderingContext {
	public:
		RenderingContext(GLFWwindow* windowHandle);
		void Init();
		void SwapBuffers();
	private:
		GLFWwindow* m_windowHandle;
	};
}