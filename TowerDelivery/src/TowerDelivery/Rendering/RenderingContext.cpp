#include "tdpch.h"
#include "RenderingContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace TowerDelivery {

	RenderingContext::RenderingContext(GLFWwindow* windowHandle) : m_windowHandle(windowHandle){
		TD_CORE_ASSERT(windowHandle, "WindowHandle is null");
	}

	void RenderingContext::Init(){
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TD_CORE_ASSERT(status, "Failed to initialize Glad!");
	}

	void RenderingContext::SwapBuffers(){
		glfwSwapBuffers(m_windowHandle);
	}
}