#pragma once

#include "Zinet/RHI/ZtRHIConfig.hpp"
#include "Zinet/RHI/ZtTypes.hpp"

#include "Zinet/Core/Assets/ZtAsset.hpp"

namespace zt::rhi::assets
{
	ZT_REFLECT_CLASS()
	class ZINET_RHI_API ShaderSource : public core::assets::Asset
	{
	public:


	protected:

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<ShaderSource>();
		ObjectBase* createCopy() const { Object* result = createCopyInternal<ShaderSource>(); *result = *this; return result; }
		
		ShaderSource() = default;
		ShaderSource(const ShaderSource& other) = default;
		ShaderSource(ShaderSource&& other) = default;
		
		~ShaderSource() noexcept = default;
		
		ShaderSource& operator = (const ShaderSource& other) = default;
		ShaderSource& operator = (ShaderSource&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "ShaderSource"; }
			constexpr static auto GetParentsClassInfo() { return std::vector{core::assets::Asset::ClassInfo{}}; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};

}