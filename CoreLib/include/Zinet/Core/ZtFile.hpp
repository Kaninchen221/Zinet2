#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtFileOpenMode.hpp"

#include <fstream>

namespace zt::core
{

	ZT_REFLECT_CLASS(NO_CONSTRUCTORS)
	class ZINET_CORE_API File
	{

	public:

		File() = default;
		File(const File& other) = delete;
		File(File&& other) = default;

		File& operator = (const File& other) = delete;
		File& operator = (File&& other) = default;

		~File() noexcept;

		void open(const FileFinder::Path& filePath, FileOpenMode openMode);

		bool isOpen() const;

		std::string readLine();

		std::string readAll();

		static std::ios_base::openmode ToStdOpenMode(FileOpenMode openMode);

		void close();

	protected:

		std::fstream fileStream;

	public:
/*GENERATED_CODE_START*/
		File& operator = (const File& other) = default;
		File& operator = (File&& other) = default;
		
		class ClassInfo : public zt::core::reflection::ClassInfo
		{
		public:
		
			std::string_view getClassName() const override { return "File"; }
			zt::core::reflection::ClassPropertiesInfos getClassPropertiesInfos() override {return zt::core::reflection::ClassPropertiesInfos(std::vector<zt::core::reflection::ClassPropertyInfo>{}); };
		
		};
		const inline static auto AddClassInfoResult = []()
		{
			auto& classesInfos = zt::core::reflection::ClassesInfos::Get();
			classesInfos.addClassInfo<ClassInfo>();
			return true;
		}();
		std::unique_ptr<zt::core::reflection::ClassInfo> getClassInfoObject() const { return std::make_unique<ClassInfo>(); }
		auto getCopyOfAllMembers() { return std::make_tuple(); };
		
/*GENERATED_CODE_END*/
	};

}