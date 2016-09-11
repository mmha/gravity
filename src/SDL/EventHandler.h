#pragma once

namespace ge::Core
{
struct EntityBase;
}

namespace ge::SDL
{
	class EventHandler
	{
	public:
		EventHandler();
		void cycle([[maybe_unused]] Core::EntityBase &, [[maybe_unused]] float);
	};
}
