#ifdef _DEBUG_
#include <stdio.h>
#endif

#include "engine.h"

Engine::Engine(){
	list = NULL;
	
	//TODO: INIT all engines..
	input = new Input(this);
	display = new Display(this);

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
	delete input;
	delete display;
}

void Engine::Quit(){
	//TODO
	running = false;
}

//this exist becouse graphics must be on the main thread
void Engine::Run(){
	running = true;

	while ( running ){
		//all the base engine does is prove a passage way to try and hand off 
		//threads
		ProcessMessages();
	}
}

void Engine::Start(){
	engine_thread = boost::thread( boost::bind( &Engine::Run, this ) );
	input_thread = boost::thread( boost::bind( &Input::Start, input ) );

	//all drawing must be made on the main thread... :/
	display->Start();
}

//NOTE: this function is called from a outside thread..
//TODO: create a tail list..
void Engine::ReceiveMessage( short classID, int messageID, void *parameters){
	eMessageList *temp = new eMessageList;
        temp->classID = classID;
	temp->messageID = messageID;
        temp->parameters = parameters;
        temp->next = NULL;

        msg_mutex.lock();
        //check to see if there is one
        if( list == NULL ){
                list = temp;
                msg_mutex.unlock();
		return;
        }

        //we loop to the bottom and add
        while( list->next )
                list = list->next;

        list->next = temp;

        msg_mutex.unlock();
}

void Engine::ProcessMessages(){
	msg_mutex.lock();
        	eMessageList *temp = list;	
		list = NULL;
        msg_mutex.unlock();

        eMessageList *next;

	
        while( temp != NULL ){
		next = temp->next;
		switch( temp->classID ){
			case SYSTEM_ENGINE: //this class so we do a bit more...
				switch( temp->messageID ){
					case QUIT: //quit
						input->ReceiveMessage( QUIT, NULL ); //no need for check for parameters with a quit message
						display->ReceiveMessage( QUIT, NULL );
						Quit();
						break;
				}
				break;
			case SYSTEM_INPUT:
				input->ReceiveMessage( temp->messageID, temp->parameters );
				break;
			case SYSTEM_DISPLAY:
				display->ReceiveMessage( temp->messageID, temp->parameters );
				break;
			default:
#ifdef _DEBUG_
				printf("ERROR: Unknown class id %i, cannot send message %i\n", temp->classID, temp->messageID );
#endif
				break;
		}

		delete temp;
                temp = next;
        }
}
