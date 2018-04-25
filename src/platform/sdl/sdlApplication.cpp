#include "sdlApplication.hpp"
#include "core/common.hpp"
#include <SDL2/SDL.h>

uint32 SDLApplication::numInstances = 0;

SDLApplication* SDLApplication::create()
{
	const uint32 flags = SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS;
	uint32 initialized = SDL_WasInit(flags);
	if(initialized != flags &&
			SDL_Init(flags) != 0) {
		DEBUG_LOG("SDLApplication", LOG_ERROR, "SDL_Init: %s", SDL_GetError());
		return nullptr;
	}

	return new SDLApplication();
}

SDLApplication::SDLApplication()
{
	numInstances++;
	isAppRunning = true;
}

SDLApplication::~SDLApplication()
{
	numInstances--;
	if(numInstances == 0) {
		SDL_Quit();
	}
}

void SDLApplication::processMessages(double delta)
{
	SDL_Event e;
	(void)delta;
	
	while(SDL_PollEvent(&e)) {
		switch(e.type){
		case SDL_QUIT:
			isAppRunning = false;
			break;
		default:
			break;
		};
	}
}

bool SDLApplication::isRunning()
{
	return isAppRunning;
}

