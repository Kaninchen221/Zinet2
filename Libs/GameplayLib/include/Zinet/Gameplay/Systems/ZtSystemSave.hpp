#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtArchive.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API SystemSave : public System
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemSave");

	public:

		SystemSave() ZINET_API_POST = default;
		SystemSave(const SystemSave& other) ZINET_API_POST = default;
		SystemSave(SystemSave&& other) ZINET_API_POST = default;
		~SystemSave() ZINET_API_POST = default;

		SystemSave& operator = (const SystemSave& other) ZINET_API_POST = default;
		SystemSave& operator = (SystemSave&& other) ZINET_API_POST = default;

		bool init() ZINET_API_POST override { return System::init();  }

		void deinit() ZINET_API_POST override { System::deinit(); }

		void imGui() ZINET_API_POST override { System::imGui(); }

		virtual bool createArchiveFromNodes() ZINET_API_POST;
		virtual bool putArchiveIntoFile() ZINET_API_POST;

		void clearCurrentBuffer() ZINET_API_POST { currentArchiveBuffer = {}; }

		virtual bool putFileIntoArchive() ZINET_API_POST;
		virtual bool recreateNodesFromArchive() ZINET_API_POST;

		void setSaveFolderPath(const core::Path& path) ZINET_API_POST { saveFolderPath = path; }
		const auto& getSaveFolderPath() const ZINET_API_POST { return saveFolderPath; }

	protected:

		bool checkSaveFolderPath() ZINET_API_POST;
		bool checkFile(const core::File& file) ZINET_API_POST;

		core::Path getSaveFilePath() ZINET_API_POST { return saveFolderPath / "save_0.temp"; }

		// Data
		core::JsonArchive::BufferT currentArchiveBuffer;

		// Config
		core::Path saveFolderPath;

	};

}