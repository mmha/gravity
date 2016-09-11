#include "Mouse.h"
#include <SDL2/SDL.h>

namespace ge::SDL
{

auto Mouse::instance() -> Mouse &
{
	static Mouse instance;
	return instance;
}

auto Mouse::relativeMovement() const -> lmi::vec2i
{
	return movements_[0];
}

auto Mouse::absolutePosition() const -> lmi::vec2i
{
	lmi::vec2i res;
	SDL_GetMouseState(&res.x, &res.y);
	return res;
}

auto Mouse::isButtonDown(Button b) const -> bool
{
	return buttonState_ & SDL_BUTTON((uint32_t)b);
}

void Mouse::setMode(Mode mode)
{
	SDL_SetRelativeMouseMode((SDL_bool)mode);
}

void Mouse::showCursor(bool value)
{
	SDL_ShowCursor(value);
}

Mouse::Mouse()
{
	SDL_AddEventWatch(
		[](void *mouseInstance, SDL_Event *event)
		{
			if(event->type != SDL_MOUSEMOTION && event->type != SDL_MOUSEBUTTONDOWN && event->type != SDL_MOUSEBUTTONUP)
			{
				return 1;
			}

			// Mouse motion update
			auto &mouse = *reinterpret_cast<Mouse *>(mouseInstance);
			lmi::vec2i delta;
			auto newButtonState = SDL_GetRelativeMouseState(&delta.x, &delta.y);
			mouse.movements_[1] += delta;

			// Mouse button update
			mouse.buttonState_ = newButtonState;
			return 0;
		},
		this);
}

void Mouse::cycle(Core::EntityBase &, float)
{
	movements_[0] = movements_[1];
	movements_[1] = {0, 0};	// That race condition should not matter...
}

}
