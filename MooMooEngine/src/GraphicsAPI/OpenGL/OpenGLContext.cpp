#include "MooMooEngine_PCH.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"

namespace MooMooEngine
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{

	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		//Glad - gets pointers to all modern (>1.1) opengl functions from the GPU driver
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CORE_ASSERT("{0} - Failed to initialize Glad!",status);

		//print the driver we are using
		LOG_INFO("Vendor:          {0}", (const char*)glGetString(GL_VENDOR));
		LOG_INFO("Renderer:        {0}", (const char*)glGetString(GL_RENDERER));
		LOG_INFO("Version OpenGL:  {0}", (const char*)glGetString(GL_VERSION));
		LOG_INFO("Version GLSL:    {0}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}
