#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::vulkan_renderer
{
	ZT_REFLECT_CLASS()
	class ZINET_VULKAN_RENDERER_API VulkanRenderer : public core::Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VulkanRenderer");

	public:

	protected:

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<VulkanRenderer>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<VulkanRenderer>(); *result = *this; return result; }
		
		VulkanRenderer() = default;
		VulkanRenderer(const VulkanRenderer& other) = default;
		VulkanRenderer(VulkanRenderer&& other) = default;
		~VulkanRenderer() noexcept = default;
		
		VulkanRenderer& operator = (const VulkanRenderer& other) = default;
		VulkanRenderer& operator = (VulkanRenderer&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "VulkanRenderer"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};
}