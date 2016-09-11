#include "Keyboard.h"
#include <SDL2/SDL.h>

namespace ge::SDL
{

auto Keyboard::operator[](int scancode) const -> bool
{
	static auto keyStates = SDL_GetKeyboardState(nullptr);
	return keyStates[scancode];
}

}
