#ifndef GE_SDL_MOUSE_H
#define GE_SDL_MOUSE_H

#include <lmi/lmi.h>
#include <cstdint>

#undef ABSOLUTE	// WTF Microsoft?
#undef RELATIVE

namespace ge::Core
{
struct EntityBase;
}

namespace ge::SDL
{
	class Mouse
	{
		public:
		enum class Mode : uint32_t
		{
			ABSOLUTE = 0,
			RELATIVE = 1
		};

		enum class Button : uint32_t
		{
			LEFT = 1,
			MIDDLE = 2,
			RIGHT = 3
		};

		static auto instance() -> Mouse &;

		auto relativeMovement() const -> lmi::vec2i;
		auto absolutePosition() const -> lmi::vec2i;
		auto isButtonDown(Button b) const -> bool;

		void setMode(Mode mode);
		void showCursor(bool value);

		void cycle([[maybe_unused]] Core::EntityBase &, [[maybe_unused]] float);

		private:
		lmi::vec2i movements_[2];	// 0: Since last frame - 1: Current frame (refreshed in a separate thread)
		uint32_t buttonState_;
		Mouse();
	};
}

#endif