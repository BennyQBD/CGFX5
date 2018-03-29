#include "sdlApplication.hpp"
#include <SDL2/SDL.h>

uint32 SDLApplication::numInstances = 0;

SDLApplication* SDLApplication::create()
{
	uint32 initialized = SDL_WasInit(SDL_INIT_EVERYTHING);
	if(initialized != SDL_INIT_EVERYTHING && 
			SDL_Init(SDL_INIT_EVERYTHING) != 0) {
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

