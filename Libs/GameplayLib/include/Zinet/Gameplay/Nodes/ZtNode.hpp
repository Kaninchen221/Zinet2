#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

namespace zt::gameplay
{
	class  Node : public core::Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::Node");

	public:

		using Children = std::vector<ObjectHandle<Node>>;

		Node() = default;
		Node(const Node& other) = default;
		Node(Node&& other) noexcept = default;
		~Node() noexcept = default;

		Node& operator = (const Node& other) = default;
		Node& operator = (Node&& other) noexcept = default;

		ObjectPtr createCopy() const override { return std::make_unique<Node>(*this); }

		std::string getClassName() const override { return "zt::gameplay::Node"; }
		auto& getChildren() noexcept { return children; }
		const auto& getChildren() const noexcept { return children; }

		void addChild(ObjectHandle<Node> ObjectHandle) { children.push_back(ObjectHandle); }

		auto begin() const noexcept { return children.begin(); }
		auto end() const noexcept { return children.end(); }

		void setParent(ObjectWeakHandle<Node> newParent) noexcept { parent = newParent; }
		auto getParent() const noexcept { return parent; }

		virtual void imGui();

		virtual void update([[maybe_unused]] float deltaTime) {}

		bool serialize(core::JsonArchive& archive) override;
		bool deserialize(core::JsonArchive& archive) override;

	protected:

		Children children;
		ObjectWeakHandle<Node> parent;

	};

}