#include "Zinet/Gameplay/Systems/ZtSystemSave.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

#include "Zinet/Core/ZtFile.hpp"

namespace fs = std::filesystem;

namespace zt::gameplay
{
	bool SystemSave::createArchiveFromNodes(ObjectHandle<Node> node)
	{
		Logger->info("Create archive", nodes.size());

		core::JsonArchive parentArchive{ &currentArchiveBuffer };
		int nodeNumber = 0;
		serializeNode(node, parentArchive, nodeNumber);

		return true;
	}

	bool SystemSave::serializeNode(ObjectHandle<Node> node, core::JsonArchive& parentArchive, int nodeNumber)
	{
		if (!node || !node->isSaveable())
			return true;

		auto* parentArchiveBuffer = parentArchive.getBuffer();
		auto& childArchiveBuffer = parentArchiveBuffer->operator[](nodeKey + std::to_string(nodeNumber));

		core::JsonArchive childArchive{ &childArchiveBuffer };
		node->serialize(childArchive);

		for (auto& childNode : node->getChildren())
		{
			++nodeNumber;

			if (!serializeNode(childNode, childArchive, nodeNumber))
				return false;
		}

		return true;
	}

	bool SystemSave::putArchiveIntoFile()
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

	bool SystemSave::putFileIntoArchive()
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
			currentArchiveBuffer = core::Json::parse(fileContent);
			//Logger->info(currentArchiveBuffer.dump(1, '\t'));
		}

		if (!checkFile(file))
			return false;

		return true;
	}

	//std::shared_ptr<Node> SystemSave::recreateNodesFromArchive() ZINET_API_POST
	//{
	//	Logger->info("Recreate nodes from archive");
	//
	//	return traverse(*currentArchiveBuffer.begin(), {});
	//}

	//std::shared_ptr<Node> SystemSave::traverse(core::Json& json, std::shared_ptr<Node> parentNode) ZINET_API_POST
	//{
	//	std::shared_ptr<Node> childNode;
	//	auto className = json.find(classNameKey);
	//	if (className != json.end())
	//	{
	//		auto& engineContext = EngineContext::Get();
	//		auto classNameValue = className->get<std::string>();
	//		Logger->info("Create node of class: {}", classNameValue);
	//
	//		auto cdo = engineContext.classRegistry.getClassByName(classNameValue);
	//		if (!cdo)
	//		{
	//			Logger->error("CDO is invalid");
	//			return {};
	//		}
	//		
	//		childNode = dynamic_pointer_cast<Node>(cdo->createCopy());
	//		if (!childNode)
	//		{
	//			Logger->error("Couldn't cast copy of cdo to Node type");
	//			return {};
	//		}
	//	}
	//	else
	//	{
	//		Logger->error("Couldn't find class name");
	//		return {};
	//	}
	//
	//	if (parentNode)
	//		parentNode->addChild(childNode);
	//
	//	core::JsonArchive archive( &json );
	//	childNode->deserialize(archive);
	//
	//	if (json.is_object()) {
	//		for (auto it = json.begin(); it != json.end(); ++it) 
	//		{
	//			if (it.key().starts_with(nodeKey))
	//			{
	//				Logger->info("Found node object: {}", it.key());
	//				traverse(*it, childNode);
	//			}
	//		}
	//	}
	//	else if (json.is_array()) 
	//	{
	//		Logger->error("Save file shouldn't contain any array");
	//	}
	//	else {
	//		// Ignore
	//	}
	//
	//	return childNode;
	//}

	bool SystemSave::checkSaveFolderPath()
	{
		if (!fs::exists(saveFolderPath))
		{
			Logger->error("Save folder doesn't exist: {}", saveFolderPath.generic_string());
			return false;
		}

		return true;
	}

	bool SystemSave::checkFile(const core::File& file)
	{
		if (!file.isOkay() || !file.isOpen())
		{
			Logger->error("Something goes wrong with file");
			return false;
		}

		return true;
	}

}