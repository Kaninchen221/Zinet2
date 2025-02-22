#include "Zinet/OpenGLRenderer/ZtOpenGLRenderer.hpp"

namespace zt::opengl_renderer
{
	bool OpenGLRenderer::init()
	{
		const int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
		if (version == 0)
		{
			Logger->error("Failed to load GL using GLAD version: {}", version);
			return false;
		}
		else
		{
			Logger->info("Succesfull load GL using GLAD");
		}

		return true;
	}

	void OpenGLRenderer::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

}