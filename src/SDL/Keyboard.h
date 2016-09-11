#pragma once

namespace ge::SDL
{
	class Keyboard
	{
		public:
		auto operator[](int scancode) const -> bool;
	};

	// TODO replace by inline once supported by a compiler
	constexpr auto keyboard = Keyboard{};
}