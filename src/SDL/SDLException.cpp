#include "SDLException.h"
#include <SDL2/SDL.h>

namespace ge::SDL
{

SDLException::SDLException()
	: msg{SDL_GetError()}
	{
	}

SDLException::SDLException(std::experimental::string_view m)
	: msg{m}
	{
	}

auto SDLException::what() const noexcept -> const char *
{
	return msg.c_str();
}

}