#include "timer.h"
#include "fontmgr.h"

Timer::Timer(){
	frames = 0;

	startTicks = SDL_GetTicks();
}

void Timer::Update(){
	frames++;

	if((SDL_GetTicks() - startTicks) >= 5000)
	{
		printf("FPS: %f\n", frames / 5.0 );
		startTicks = SDL_GetTicks();
		frames = 0;
	}
}
