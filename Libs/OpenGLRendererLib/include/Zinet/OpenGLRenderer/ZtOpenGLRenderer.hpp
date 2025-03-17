#pragma once

#include "Zinet/OpenGLRenderer/ZtOpenGLRendererConfig.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::opengl_renderer
{
	ZT_REFLECT_CLASS()
	class ZINET_OPENGL_RENDERER_API OpenGLRenderer : public core::Object
	{
	protected:

		inline static zt::core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("OpenGLRenderer");

		static void WindowResized(void* renderer, const Vector2ui& size) { glViewport(0, 0, size.x, size.y); }

	public:

		bool init(wd::Window& window);

		void preRender();
		void postRender();
		void render();

		void setupTexture(const Vector2<GLsizei>& size, const void* data);

		const char* getVertexShaderSource() const;
		const char* getFragmentShaderSource() const;

		GLuint compileShader(GLenum type, const char* source) const;

		void setViewportSize(Vector2i size);

	protected:

		ZT_REFLECT_MEMBER(ReadWrite)
		Vector4f clearColor = Vector4f{ 0.f, 0.f, 0.f, 1.f };

		const std::array<float, 16> vertices = std::array {
			// Positions  // TexCoords
			-1.0f, 1.0f,  0.0f, 0.0f,
			 1.0f, 1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 1.0f
		};
		const std::array<GLuint, 6u> indices = std::array{ 0u, 1u, 3u, 1u, 2u, 3u };

		GLuint vertexShader;
		GLuint fragmentShader;

		GLuint shaderProgram;

		GLuint VBO, VAO, EBO;

		GLuint textureID;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<OpenGLRenderer>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<OpenGLRenderer>(); *result = *this; return result; }
		
		OpenGLRenderer() = default;
		OpenGLRenderer(const OpenGLRenderer& other) = default;
		OpenGLRenderer(OpenGLRenderer&& other) = default;
		~OpenGLRenderer() noexcept = default;
		
		OpenGLRenderer& operator = (const OpenGLRenderer& other) = default;
		OpenGLRenderer& operator = (OpenGLRenderer&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "OpenGLRenderer"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(clearColor)& getClearColor() const { return clearColor; }
		decltype(clearColor)& getClearColor() { return clearColor; }
		void setClearColor(const decltype(clearColor)& newValue) { clearColor = newValue; }
		
/*GENERATED_CODE_END*/
	};
}