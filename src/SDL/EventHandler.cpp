#include "EventHandler.h"
#include <SDL2/SDL.h>
#include <cstdlib>

namespace ge::SDL
{

EventHandler::EventHandler()
{
	SDL_AddEventWatch([](void *, SDL_Event *event)
	{
		if (event->type == SDL_QUIT)
		{
			std::exit(EXIT_SUCCESS);
		}

		return 1;
	}, nullptr);
}

void EventHandler::cycle(Core::EntityBase &, float)
{
	SDL_PumpEvents();
	SDL_FilterEvents([](void *, SDL_Event *) { return 0; }, nullptr); //Clear the event queue
}

}
