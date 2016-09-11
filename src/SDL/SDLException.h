#ifndef GE_SDL_EXCEPTION_H
#define GE_SDL_EXCEPTION_H

#include <exception>
#include <experimental/string_view>
#include <string>

namespace ge::SDL
{
	class SDLException : public std::exception
	{
	public:
		SDLException();
		SDLException(std::experimental::string_view m);
		auto what() const noexcept -> const char *;
	private:
		std::string msg;
	};
}

#endif