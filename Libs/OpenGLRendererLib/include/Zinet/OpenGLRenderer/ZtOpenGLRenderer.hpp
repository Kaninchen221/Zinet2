#pragma once

#include "Zinet/OpenGLRenderer/ZtOpenGLRendererConfig.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::opengl_renderer
{
	ZT_REFLECT_CLASS()
	class ZINET_OPENGL_RENDERER_API OpenGLRenderer : public core::Object
	{
	protected:

		inline static zt::core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("OpenGLRenderer");

	public:

		bool init();

		void render();

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
			constexpr static auto GetParentsClassInfo() { return std::vector{core::Object::ClassInfo{}}; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};
}