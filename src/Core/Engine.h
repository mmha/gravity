#pragma once

#include "Subsystem.h"
#include "Entity.h"
#include <boost/hana.hpp>
#include <chrono>
#include <memory>

namespace ge::Core
{
	template<Subsystem... S>
	class Engine
	{
		public:
		Engine(EntityBase *&&rootEntity, S &&... subsystems)
		: subsystems_{std::forward<S>(subsystems)...}
		, root_{rootEntity}
		{
		}

		auto run() -> int
		{
			auto tLast = std::chrono::steady_clock::now();
			while(true)
			{
				auto tNow = std::chrono::steady_clock::now();
				auto dt = std::chrono::duration<float>{tNow - tLast};
				cycle(dt.count());
				tLast = tNow;
			}
		}

		auto rootEntity() -> EntityBase &
		{
			return root_;
		}

		private:
		void cycle(float dt)
		{
			boost::hana::for_each(subsystems_, [this, dt](auto &subsystem)
			{
				subsystem.cycle(*root_, dt);
			});
		}

		boost::hana::tuple<S...> subsystems_;
		std::unique_ptr<EntityBase> root_;
	};
}
