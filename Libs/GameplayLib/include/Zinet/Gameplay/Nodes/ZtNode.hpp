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

		Node() = default;
		Node(const Node& other) = default;
		Node(Node&& other) noexcept = default;
		~Node() noexcept = default;

		Node& operator = (const Node& other) = default;
		Node& operator = (Node&& other) noexcept = default;

		ObjectPtr createCopy() const override { return std::make_unique<Node>(*this); }

		std::string getClassName() const override { return "zt::gameplay::Node"; }

		using Children = std::vector<ObjectHandle<Node>>;
		auto& getChildren() noexcept { return data.children; }
		const auto& getChildren() const noexcept { return data.children; }

		void addChild(ObjectHandle<Node> ObjectHandle) { data.children.push_back(ObjectHandle); }

		auto begin() const noexcept { return data.children.begin(); }
		auto end() const noexcept { return data.children.end(); }

		void setParent(ObjectWeakHandle<Node> newParent) noexcept { data.parent = newParent; }
		auto getParent() const noexcept { return data.parent; }

		virtual void imGui();

		virtual void update([[maybe_unused]] float deltaTime) {}

		bool serialize(core::JsonArchive& archive) override;
		bool deserialize(core::JsonArchive& archive) override;

	protected:

		struct {
			Children children;
			ObjectWeakHandle<Node> parent;
		} data;

	};

}