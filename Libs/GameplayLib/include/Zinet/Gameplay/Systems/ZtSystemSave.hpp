#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtArchive.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::gameplay
{
	class  SystemSave : public System
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemSave");

	public:

		SystemSave() = default;
		SystemSave(const SystemSave& other) = default;
		SystemSave(SystemSave&& other) noexcept = default;
		~SystemSave() noexcept = default;

		SystemSave& operator = (const SystemSave& other) = default;
		SystemSave& operator = (SystemSave&& other) noexcept = default;

		bool init() override { return System::init();  }

		void deinit() override { System::deinit(); }

		void imGui() override { System::imGui(); }

		virtual bool createArchiveFromNodes(ObjectHandle<Node> node);
		virtual bool putArchiveIntoFile();

		void clearCurrentBuffer() { currentArchiveBuffer = {}; }

		virtual bool putFileIntoArchive();
	
		//virtual std::shared_ptr<Node> recreateNodesFromArchive() ZINET_API_POST;

		void setSaveFolderPath(const core::Path& path) { saveFolderPath = path; }
		const auto& getSaveFolderPath() const noexcept { return saveFolderPath; }

	protected:

		bool serializeNode(ObjectHandle<Node> nodeHandle, core::JsonArchive& archive, int nodeNumber);

		//std::shared_ptr<Node> traverse(core::Json& json, std::shared_ptr<Node> parentNode) ZINET_API_POST;

		bool checkSaveFolderPath();
		bool checkFile(const core::File& file);

		core::Path getSaveFilePath() { return saveFolderPath / "save_0.temp"; }

		struct {
			core::JsonArchive::BufferT currentArchiveBuffer;
			core::Path saveFolderPath;
			std::string nodeKey = "node_";
			std::string classNameKey = "className";
		} data;
		core::JsonArchive::BufferT& currentArchiveBuffer = data.currentArchiveBuffer;

		core::Path& saveFolderPath = data.saveFolderPath;
		std::string& nodeKey = data.nodeKey;
		std::string& classNameKey = data.classNameKey;

	};

}