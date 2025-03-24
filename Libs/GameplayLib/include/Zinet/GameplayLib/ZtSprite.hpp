#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"
#include "Zinet/GameplayLib/ZtNode.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::gameplay_lib
{
	namespace sf = software_renderer;

	ZT_REFLECT_CLASS()
	class ZINET_GAMEPLAY_LIB_API Sprite : public Node
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("Sprite");

	public:

		sf::DrawInfo getDrawInfo() const override;

	protected:

		ZT_REFLECT_MEMBER(ReadWrite)
		RectF textureRegion;

		ZT_REFLECT_MEMBER(ReadWrite)
		sf::RenderTarget texture;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<Sprite>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<Sprite>(); *result = *this; return result; }
		
		Sprite() = default;
		Sprite(const Sprite& other) = default;
		Sprite(Sprite&& other) = default;
		~Sprite() noexcept = default;
		
		Sprite& operator = (const Sprite& other) = default;
		Sprite& operator = (Sprite&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "Sprite"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(textureRegion)& getTextureRegion() const { return textureRegion; }
		decltype(textureRegion)& getTextureRegion() { return textureRegion; }
		void setTextureRegion(const decltype(textureRegion)& newValue) { textureRegion = newValue; }
		
		const decltype(texture)& getTexture() const { return texture; }
		decltype(texture)& getTexture() { return texture; }
		void setTexture(const decltype(texture)& newValue) { texture = newValue; }
		
/*GENERATED_CODE_END*/
	};
}