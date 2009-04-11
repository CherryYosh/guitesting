#ifdef _DEBUG_
#include <stdio.h>
#endif

#include "engine.h"
#include "input.h"
#include "display.h"

Engine* Engine::engine;

Engine::Engine(){
	engine = this;

	printf( "engine is!!! %p\n", engine );
	
	input = new Input();
	display = new Display();

	input->BindAction( "default", SYSTEM_ENGINE, "QUIT", true, QUIT, NULL );
	input->BindKey( "default", SDLK_q, KMOD_LCTRL, "QUIT" );
	input->BindKey( "default", SDLK_q, KMOD_NONE, "QUIT" );

	input->BindAction( "typing", SYSTEM_ENGINE, "QUIT", true, QUIT, NULL );
	input->BindKey( "typing", SDLK_q, KMOD_LCTRL, "QUIT" );
	

////////////////////////////////////
// all this does is set up the basic
// keyboard functions... waistful memory :[
// TODO: find a better way???
	double* data = new double[ 3 ];
	data[0] = 0.0; data[1] = 0.0; data[2] =-1.0;
	input->BindAction( "default", SYSTEM_DISPLAY, "CAMERA_MOVEFORWARD", true, CAMERA_MOVE, (void*)data );
	input->BindKey( "default", SDLK_w, KMOD_NONE, "CAMERA_MOVEFORWARD" );

	data = new double[3]; //seems waistful dont it -_-
	data[0] = 0.0; data[1]=0.0; data[2] = 1.0;
	input->BindAction( "default", SYSTEM_DISPLAY, "CAMERA_MOVEBACKWARDS", true, CAMERA_MOVE, (void*)data );
	input->BindKey( "default", SDLK_s, KMOD_NONE, "CAMERA_MOVEBACKWARDS" );

        data = new double[3];
        data[0] = -1.0; data[1]=0.0; data[2] = 0.0;
        input->BindAction( "default", SYSTEM_DISPLAY, "CAMERA_MOVELEFT", true, CAMERA_MOVE, (void*)data );
        input->BindKey( "default", SDLK_a, KMOD_NONE, "CAMERA_MOVELEFT" );

        data = new double[3];
        data[0] = 1.0; data[1]=0.0; data[2] = 0.0;
        input->BindAction( "default", SYSTEM_DISPLAY, "CAMERA_MOVERIGHT", true, CAMERA_MOVE, (void*)data );
        input->BindKey( "default", SDLK_d, KMOD_NONE, "CAMERA_MOVERIGHT" );
}

Engine::~Engine(){
	printf( "deleting engine\n" );
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
	}
}

void Engine::Start(){
	Run();

	delete this;
}
