#pragma once

#include "Entity.h"
#include <bibi/Core.h>

namespace ge::Core
{
	template<typename S>
	concept bool Subsystem =
	bibi::MoveConstructible<S> &&
	requires(S s, EntityBase &e, float dt)
	{
		{s.cycle(e, dt)};
	};
}
