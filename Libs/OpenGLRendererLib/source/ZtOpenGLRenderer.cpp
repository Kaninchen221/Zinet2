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

	void OpenGLRenderer::preRender()
	{
		vertexShader = compileShader(GL_VERTEX_SHADER, getVertexShaderSource());
		fragmentShader = compileShader(GL_FRAGMENT_SHADER, getFragmentShaderSource());

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glUseProgram(shaderProgram);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void OpenGLRenderer::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void OpenGLRenderer::postRender()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);
		glDeleteTextures(1, &textureID);
	}

	const char* OpenGLRenderer::getVertexShaderSource() const
	{
		return R"(
			#version 330 core
			layout (location = 0) in vec2 pos;
			layout (location = 1) in vec2 texCoord;
			out vec2 TexCoord;
			void main() {
				gl_Position = vec4(pos, 0.0, 1.0);
				TexCoord = texCoord;
			}
		)";
	}

	const char* OpenGLRenderer::getFragmentShaderSource() const
	{
		return R"(
			#version 330 core
			in vec2 TexCoord;
			out vec4 FragColor;
			uniform sampler2D imageTexture;
			void main() {
				FragColor = texture(imageTexture, TexCoord);
			}
		)";
	}

	GLuint OpenGLRenderer::compileShader(GLenum type, const char* source) const
	{
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);
		return shader;
	}

	void OpenGLRenderer::setupTexture(const Vector2<GLsizei>& size, const void* data)
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load the image into the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

}