#include "Window.h"
#include <SDL2/SDL_syswm.h>
#ifdef __linux__
	#include <GL/glx.h>
#endif

namespace ge::SDL
{

Window::Window(const std::string &title, lmi::vec2ui resolution, VSync useVsync, Fullscreen useFullscreen)
: window_{nullptr, &SDL_DestroyWindow}
, context_{nullptr, &SDL_GL_DeleteContext}
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetSwapInterval(static_cast<int>(useVsync));

	window_.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolution.x, resolution.y, SDL_WINDOW_OPENGL));
	if(!window_)
	{
		throw SDLException{};
	}

	SDL_SetWindowFullscreen(window_.get(), static_cast<int>(useFullscreen));

	context_.reset(SDL_GL_CreateContext(window_.get()));
	if(!context_)
	{
		throw SDLException{};
	}
}

void Window::makeCurrent()
{
	SDL_GL_MakeCurrent(window_.get(), context_.get());
}

void Window::swapBuffers()
{
	SDL_GL_SwapWindow(window_.get());
}

auto Window::getNativeHandle() const -> NativeHandle
{
	SDL_SysWMinfo wmInfo;
	SDL_GetWindowWMInfo(window_.get(), &wmInfo);
#if _WIN32
	return {reinterpret_cast<uint64_t>(wmInfo.info.win.window), reinterpret_cast<uint64_t>(wglGetCurrentContext())};
#elif __linux__
	return {reinterpret_cast<uint64_t>(wmInfo.info.x11.window), reinterpret_cast<uint64_t>(glXGetCurrentContext())};
#else
#error "Unimplemented Window::getNativeHandle()"
#endif
}

auto Window::getSize() const -> lmi::vec2ui
{
	lmi::vec2ui res;
	SDL_GetWindowSize(window_.get(), reinterpret_cast<int32_t *>(&res.x), reinterpret_cast<int32_t *>(&res.y));
	return res;
}

}
