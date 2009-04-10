#ifdef _DEBUG_
#include <stdio.h>
#endif

#include "engine.h"
#include "input.h"
#include "display.h"

Engine::Engine(){
	Messages = NULL;
	engine = this;

	//TODO: INIT all engines..
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
	engine = NULL;
	delete input;
	delete display;
}

void Engine::Quit(){
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

	printf( "balh!\n" );
	engine_thread.join();
	input_thread.join();
	printf( "here!\n" );
}

//NOTE: this function is called from a outside thread..
//TODO: create a tail list..
void Engine::ReceiveMessage( short classID, int messageID, void *parameters){
	Engine_Message* temp = new Engine_Message;
        temp->classID = classID;
	temp->messageID = messageID;
        temp->parameters = parameters;

        msg_mutex.lock();

	if( Messages == NULL )
		Messages = new std::vector<Engine_Message*>;

	Messages[0].push_back( temp );

        msg_mutex.unlock();
}

void Engine::ProcessMessages(){
	msg_mutex.lock();
		std::vector<Engine_Message*>* temp = Messages;	
		Messages = NULL;
        msg_mutex.unlock();

	if( temp == NULL )
		return;

        Engine_Message* msg;
	unsigned int size = temp[0].size();	
        for( unsigned int i = 0; i < size; i++ ){
		msg = temp[0][i];
		switch( msg->classID ){
			case SYSTEM_ENGINE: //this class so we do a bit more...
				printf ("here\n" );
				switch( msg->messageID ){
					case QUIT: //quit
						printf( "here 5\n" );
						input->ReceiveMessage( QUIT, NULL ); //no need for check for parameters with a quit message
						display->ReceiveMessage( QUIT, NULL );
						Quit();
						break;
				}
				break;
			case SYSTEM_INPUT:
				input->ReceiveMessage( msg->messageID, msg->parameters );
				break;
			case SYSTEM_DISPLAY:
				display->ReceiveMessage( msg->messageID, msg->parameters );
				break;
			default:
#ifdef _DEBUG_
				printf("ERROR: Unknown class id %i, cannot send message %i\n", msg->classID, msg->messageID );
#endif
				break;
		}
        }

	temp[0].clear();
	delete [] temp;
}
