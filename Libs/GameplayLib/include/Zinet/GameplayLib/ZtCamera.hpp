#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"
#include "Zinet/GameplayLib/ZtNode.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::gameplay_lib
{
	namespace sf = software_renderer;

	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API Camera : public Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("Camera");

	public:

		void create(const Vector2i& newSize = { 512, 512 });

		// The lookAt value is the position member
		// The pivot of the camera should be always considered in the midle of the camera
		void setLookAt(const Vector2f& lookAtPosition) { position = lookAtPosition; }
		const Vector2f& getLookAt() const { return position; }

		Vector2f getSize() const { return viewportRenderTarget.getResolution(); }

		Vector2f mousePositionNormToWorld(const Vector2f& mousePositionNorm) const;

	protected:

		ZT_REFLECT_MEMBER(ReadWrite)
		sf::RenderTarget viewportRenderTarget;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<Camera>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<Camera>(); *result = *this; return result; }
		
		Camera() = default;
		Camera(const Camera& other) = default;
		Camera(Camera&& other) = default;
		~Camera() noexcept = default;
		
		Camera& operator = (const Camera& other) = default;
		Camera& operator = (Camera&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "Camera"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(viewportRenderTarget)& getViewportRenderTarget() const { return viewportRenderTarget; }
		decltype(viewportRenderTarget)& getViewportRenderTarget() { return viewportRenderTarget; }
		void setViewportRenderTarget(const decltype(viewportRenderTarget)& newValue) { viewportRenderTarget = newValue; }
		
/*GENERATED_CODE_END*/
	};
}