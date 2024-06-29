#pragma once

#include "zinet/lib_name/macros.hpp"

ZT_REFLECT_NAMESPACE()
namespace zt::engine
{
	ZT_REFLECT_CLASS()
	class ZINET_ENGINE_API Asset : public Asset
	{
		ZT_REFLECT_METHOD()
		void update() { /* ... */ }

		ZT_REFLECT_MEMBER()
		int count;

/*GENERATED_CODE_START*/ 
		
		Asset() = default;
		Asset(const Asset& other) = default;
		Asset(Asset&& other) = default;
		
		~Asset() noexcept = default;
		
		Asset& operator = (const Asset& other) = default;
		Asset& operator = (Asset&& other) = default;
		 
		
		class ClassInfo
		{
		public:
		
			static std::string_view GetClassName() { return "Asset"; }
		};
/*GENERATED_CODE_END*/

	};
}