#ifdef _DEBUG_
#include <stdio.h>
#endif

#include "engine.h"
#include "input.h"
#include "display.h"

Engine* Engine::engine;

Engine::Engine(){
	engine = this;

	display = new Display();
	input = new Input();

	input->BindAction( "default", "QUIT", true, boost::bind<void>( &Engine::Quit, Engine::engine ) );
	input->BindKey( "default", SDLK_q, KMOD_LCTRL, "QUIT" );
	input->BindKey( "default", SDLK_q, KMOD_NONE, "QUIT" );

	input->BindAction( "typing", "QUIT", true, boost::bind<void>( &Engine::Quit, Engine::engine ) );
	input->BindKey( "typing", SDLK_q, KMOD_LCTRL, "QUIT" );
	

////////////////////////////////////
// all this does is set up the basic
// keyboard functions... waistful memory :[
// TODO: find a better way???
/*
	input->BindAction( "default", SYSTEM_DISPLAY, "CAMERA_MOVEFORWARD", true, CAMERA_MOVE, (void*)data );
	input->BindKey( "default", SDLK_w, KMOD_NONE, "CAMERA_MOVEFORWARD" );

	input->BindAction( "default", SYSTEM_DISPLAY, "CAMERA_MOVEBACKWARDS", true, CAMERA_MOVE, (void*)data );
	input->BindKey( "default", SDLK_s, KMOD_NONE, "CAMERA_MOVEBACKWARDS" );

        input->BindAction( "default", SYSTEM_DISPLAY, "CAMERA_MOVELEFT", true, CAMERA_MOVE, (void*)data );
        input->BindKey( "default", SDLK_a, KMOD_NONE, "CAMERA_MOVELEFT" );

        input->BindAction( "default", SYSTEM_DISPLAY, "CAMERA_MOVERIGHT", true, CAMERA_MOVE, (void*)data );
        input->BindKey( "default", SDLK_d, KMOD_NONE, "CAMERA_MOVERIGHT" );
*/
}

Engine::~Engine(){
	delete input;
	delete display;
	engine = NULL;
}

void Engine::Quit(){
	running = false;
}

void Engine::Run(){
	running = true;

	while ( running ){
		display->Start();
		input->Start();
	
		//TODO: REMOVE THIS!!!
		//NOTE: this is a hack to fix a trailing bug!
#ifdef _DEBUG_
		SDL_Delay( 10 );
#endif
	
	}
}

void Engine::Start(){
	Run();

	delete this;
}
