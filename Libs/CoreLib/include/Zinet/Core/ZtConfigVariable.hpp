#pragma once

#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtArchive.hpp"

namespace zt::core
{
	// TODO: Write some logic to save/load it to a specific file
	template<class ValueType>
	class ConfigVariable : public Object
	{
	public:

		using ValueT = ValueType;

		ConfigVariable() = delete;
		ConfigVariable(const std::string& newDisplayName, ValueT&& defaultValue, const std::string& newToolTip)
			: Object{ newDisplayName },
			value{ defaultValue },
			toolTip{ newToolTip }
		{ }
		ConfigVariable(const ConfigVariable& other) = default;
		ConfigVariable(ConfigVariable&& other) noexcept = default;
		~ConfigVariable() noexcept = default;

		ConfigVariable& operator = (const ConfigVariable& other) = default;
		ConfigVariable& operator = (ConfigVariable&& other) noexcept = default;

		ObjectPtr createCopy() const override { return std::make_unique<ConfigVariable>("", ValueT{}, ""); }

		bool serialize(JsonArchive& archive) override;

		bool deserialize(JsonArchive& archive) override;

		std::string getClassName() const override { return "zt::core::ConfigVariable"; }

		void imGui() override {}

		const auto& getToolTip() const noexcept { return toolTip; }

		const auto& getValue() const noexcept { return value; }

	protected:

		std::string toolTip;
		ValueT value;

	};

	template<class ValueType>
	bool ConfigVariable<ValueType>::serialize(JsonArchive& archive)
	{
		archive.serialize(fmt::format("cvar_{}", displayName), value);
		return true;
	}

	template<class ValueType>
	bool ConfigVariable<ValueType>::deserialize(JsonArchive& archive)
	{
		archive.deserialize(fmt::format("cvar_{}", displayName), value);
		return true;
	}

	template<class T>
	using CVar = ConfigVariable<T>;
}