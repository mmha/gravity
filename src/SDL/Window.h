#pragma once

#include "SDLException.h"
#include "../Core/Sigslot.h"
#include <lmi/lmi.h>
#include <SDL2/SDL.h>
#include <memory>

namespace ge::SDL
{
	enum class VSync
	{
		OFF, ON
	};

	enum class Fullscreen
	{
		OFF,
		FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
		FULLSCREEN = SDL_WINDOW_FULLSCREEN
	};

	struct NativeHandle
	{
		uint64_t window, context;
	};

	class Window
	{
	public:
		Window(const std::string &title, lmi::vec2ui resolution, VSync useVsync = VSync::OFF, Fullscreen useFullscreen = Fullscreen::OFF);

		void makeCurrent();
		void swapBuffers();
		auto getNativeHandle() const -> NativeHandle;

		auto getSize() const -> lmi::vec2ui;

		Core::Signal<Core::single_threaded, lmi::vec2ui> resized;
	private:
		using WindowPtr = std::unique_ptr<SDL_Window, decltype(SDL_DestroyWindow) *>;
		using ContextPtr = std::unique_ptr<void, decltype(SDL_GL_DeleteContext) *>;
		WindowPtr window_;
		ContextPtr context_;
	};
}
