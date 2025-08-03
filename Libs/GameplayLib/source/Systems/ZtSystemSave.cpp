#include "Zinet/Gameplay/Systems/ZtSystemSave.hpp"

#include <imgui.h>

#include "Zinet/Core/ZtFile.hpp"

namespace fs = std::filesystem;

namespace zt::gameplay
{
	bool SystemSave::createArchiveFromNodes() ZINET_API_POST
	{
		Logger->info("Create archive from {} nodes", nodes.size());

		for (auto& weakNode : nodes)
		{
			if (auto node = weakNode.lock())
			{
				auto& nodeArchiveBuffer = currentArchiveBuffer[node->getDisplayName()];
				core::JsonArchive archive{ &nodeArchiveBuffer };
				node->serialize(archive);
			}
		}

		return true;
	}

	bool SystemSave::putArchiveIntoFile() ZINET_API_POST
	{
		Logger->info("Put archive into file", nodes.size());

		if (!checkSaveFolderPath())
			return false;

		const core::Path saveFilePath = getSaveFilePath();

		core::File file;
		file.open(saveFilePath, core::FileOpenMode::Write);
		if (file.isOpen())
		{
			auto archiveDump = currentArchiveBuffer.dump(1, '\t');
			//Logger->info(archiveDump);

			file.write(archiveDump);
		}

		if (!checkFile(file))
			return false;

		return true;
	}

	bool SystemSave::putFileIntoArchive() ZINET_API_POST
	{
		Logger->info("Put file into archive", nodes.size());

		if (!checkSaveFolderPath())
			return false;

		const auto saveFilePath = getSaveFilePath();
		if (!fs::exists(saveFilePath))
		{
			Logger->error("Save file doesn't exist");
			return false;
		}

		core::File file;
		file.open(saveFilePath, core::FileOpenMode::Read);
		if (file.isOpen())
		{
			auto fileContent = file.readAll();
			currentArchiveBuffer = fileContent;
			Logger->info(currentArchiveBuffer.dump(1, '\t'));
		}

		if (!checkFile(file))
			return false;

		return true;
	}

	bool SystemSave::recreateNodesFromArchive() ZINET_API_POST
	{

		return true;
	}

	bool SystemSave::checkSaveFolderPath() ZINET_API_POST
	{
		if (!fs::exists(saveFolderPath))
		{
			Logger->error("Save folder doesn't exist: {}", saveFolderPath.generic_string());
			return false;
		}

		return true;
	}

	bool SystemSave::checkFile(const core::File& file) ZINET_API_POST
	{
		if (!file.isOkay() || !file.isOpen())
		{
			Logger->error("Something goes wrong with file");
			return false;
		}

		return true;
	}
}