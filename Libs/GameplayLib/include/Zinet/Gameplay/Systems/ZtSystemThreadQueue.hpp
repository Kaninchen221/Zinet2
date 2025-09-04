#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <functional>

namespace zt::gameplay
{
	class  SystemThreadQueue : public System
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemThreadQueue");

	public:

		using ObjectHandleT = ObjectHandle<core::Object>;

		SystemThreadQueue() = default;
		SystemThreadQueue(const SystemThreadQueue& other) = default;
		SystemThreadQueue(SystemThreadQueue&& other) noexcept = default;
		~SystemThreadQueue() noexcept = default;

		SystemThreadQueue& operator = (const SystemThreadQueue& other) = default;
		SystemThreadQueue& operator = (SystemThreadQueue&& other) noexcept = default;

		void update() override;

		template<class Invocable>
		void add(ObjectHandleT objectHandle, Invocable invocable)
		{
			std::lock_guard<std::mutex> guard(mutex);

			Logic logic = invocable;
			events.emplace_back(objectHandle, logic);
		}

		size_t getEventsCount() const noexcept { return events.size(); }

	protected:

		using Logic = std::function<void(ObjectHandleT)>;
		using Event = std::pair<ObjectHandleT, Logic>;
		std::vector<Event> events;

		std::mutex mutex;
	};

}