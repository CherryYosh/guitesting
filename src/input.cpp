/*   James Stevenson 09/27/08
 *******************************************************************
 * A full function, threaded, SDL input class including mouse support
 * A action is bound before a key is bound to that action, including 
 * a optional mod key.
 * multipul keys may be bound to a single action without fear
 * key may be rebound without need to restart, actions however cant
 ******************************************************************
 * TODO: add unicode controler support, along with support for the 
 * odditys found in some key boards / OS's (no ctrls in OSX, ) also
 * support for additional mouse buttons.. 
 */

#include "input.h"
#include "engine.h"


Input::Input( Engine *ptEngine ) : System(ptEngine){
	//create a few empty profiles
	Input_ProfileDataT* temp = new Input_ProfileDataT;
	temp->Name = "default";
	Profiles.push_back( temp );

	temp = new Input_ProfileDataT;
	temp->Name = "typing";
	Profiles.push_back( temp );

	SetProfile( "default" );
}

Input::~Input(){
	//TODO
}

/*
 * Starts the class, key and mouse events are polled and processed
 * before messages are sent out
 */
void Input::Start(){
	SDL_Event keyevent;
	running = true;

	while( running ){
		while( SDL_PollEvent( &keyevent ) ){
			switch( keyevent.type ){
				case SDL_KEYDOWN:
					ProcessKey( true, keyevent.key.keysym );//, keyevent.key.keysym.mod );
					break;
				case SDL_KEYUP:
					ProcessKey( false, keyevent.key.keysym );//.sym, keyevent.key.keysym.mod );
					break;
				case SDL_MOUSEBUTTONDOWN:
					engine->ReceiveMessage( SYSTEM_DISPLAY, MOUSE_PRESS, NULL );
					break;
				case SDL_MOUSEBUTTONUP:
					engine->ReceiveMessage( SYSTEM_DISPLAY, MOUSE_RELEASE, NULL );
					break;
				case SDL_MOUSEMOTION:
					//engine->ReceiveMessage( SYSTEM_DISPLAY, MOUSE_MOTION, NULL );
					break;
				case SDL_ACTIVEEVENT:
					break;
				case SDL_VIDEOEXPOSE:
					//NOTE: we dont need to do anything here becouse the screen will be redrawn pretty fast anyways..
					break;
				case SDL_VIDEORESIZE:{ //note the { to allow varible creation
					unsigned int* data = new unsigned int[2];
					data[0] = keyevent.resize.w;
					data[1] = keyevent.resize.h;
					engine->ReceiveMessage( SYSTEM_DISPLAY, WINDOW_RESIZE, (void*)data );
					}break;
				case SDL_QUIT:
					running = false;
					engine->ReceiveMessage( SYSTEM_ENGINE, QUIT, NULL );
					break;
				default:
#ifdef _DEBUG_
					printf( "ERROR: Unkown sdl event type %i presented to Input\n", keyevent.type );
#endif
					break;
			}
		}
	
		ProcessInput();
		ProcessMessages();
	}
}

void Input::ProcessMessages(){
	//this should be farily fast (not so blocking) 
	msg_mutex.lock();
		MessageList *temp = msgList;
		msgList = NULL;
	msg_mutex.unlock();

	MessageList *next;

	while( temp ){
		next = temp->next;
		
		switch( temp->ID ){
			case QUIT: //quit
				running = false;
				break;
			case INPUT_CHANGE_PROFILE:
				SetProfile( ((std::string*)temp->parameters)[0] );
				break;
			default:
#ifdef _DEBUG_
				printf( "ERROR: Unknown message id (%i) presented to input!\n", temp->ID );
#endif
				break;
		}

		if( temp->parameters != NULL )
			delete [] (char*)temp->parameters;
		delete temp;
		temp = next;
	}
}

//binds the action to a classid and a function
//NOTE: cannot be rebound, and there is no check for multipul calls
void Input::BindAction( std::string profile, short id, std::string action, bool useOnce, int functionID, void* parameters){
	Input_ActionDataT *newAction = new Input_ActionDataT;

	newAction->id = id;
	newAction->Name = action;
	newAction->Count = 0;
	newAction->UseOnce = useOnce;
	newAction->Handled = false;
	newAction->FunctionID = functionID;
	newAction->Parameters = parameters;

	//get the profile
	size_t size = Profiles.size();
	for( unsigned int i = 0; i < size; i++ ){
		if( Profiles[i]->Name == profile ){
			Profiles[i]->Actions.push_back( newAction );
		}
	}
}

void Input::BindKey( std::string profile, SDLKey key, SDLMod mod, std::string action ){
	Input_ProfileDataT* prof = GetProfile( profile );
	std::vector<Input_ActionDataT*> actions = prof->Actions;
	Input_ActionDataT* a;

	//make sure the action is bound, if it isnt we return
	size_t size = actions.size();
	for( unsigned int i = 0; i < size; i++ ){
		if( actions[i]->Name == action ){
			a = actions[i];
			break;
		}
	}

	if( a == NULL ){
#ifdef _DEBUG_
		printf( "ERROR: Unable to bind key (%i) to %s, action %s not found!\n", key, profile.c_str(), action.c_str() );
#endif
		return;
	}

	Input_KeyDataT* newKey = new Input_KeyDataT;
	newKey->Key = key;
	newKey->Mod = mod;
	newKey->Action = a;
	
	prof->Keys.push_back( newKey );
}

void Input::ProcessKey( bool pressed, SDL_keysym sym ){//SDLKey key, SDLMod mod ){
	std::vector<Input_KeyDataT*> keys = ActiveProfile->Keys;

	size_t size = keys.size();
	for( unsigned int i = 0; i < size; i++ ){
		//				this allows use to have caps / numlock on while typeing
		if( keys[i]->Key == sym.sym && ( sym.mod & ~( KMOD_NUM | KMOD_CAPS ) ) == keys[i]->Mod ){
			if( pressed )
				keys[i]->Action->Count++;
			else{
				if( keys[i]->Action->Count > 0 )
					keys[i]->Action->Count--;
				//reset the handeld flag if no keys are down
				else if( keys[i]->Action->UseOnce )
					keys[i]->Action->Handled = false;
			}
			//auidios.. 
			return;
		}
	}

	//else we do this
	if( pressed && ActiveProfile->Name == "typing" ){
		unsigned short* data = new unsigned short[1];
		data[0] = sym.unicode;
		//data[1] = mod;
		engine->ReceiveMessage( SYSTEM_DISPLAY, INPUT_KEYPRESS, (void*)data );
	}
}

void Input::ProcessInput(){
	std::vector<Input_ActionDataT*> actions = ActiveProfile->Actions;
	Input_ActionDataT* action;

	size_t size = actions.size();
	for( unsigned int i = 0; i < size; i++ ){
		action = actions[i];
		if( action->Count > 0 && !action->Handled ){
			//just a little sanity check, not sure why this would happen
			if( action->id == SYSTEM_INPUT ){
				this->ReceiveMessage( action->FunctionID, action->Parameters );
			} else {
				engine->ReceiveMessage( action->id, action->FunctionID, action->Parameters );
			}

			if( action->UseOnce )
				action->Handled = true;
		}
	}
}

Input_ProfileDataT* Input::GetProfile( std::string name ){
	size_t size = Profiles.size();
	for( unsigned int i = 0; i < size; i++ ){
		if( Profiles[i]->Name == name )
			return Profiles[i];
	}
	return NULL;
}

bool Input::SetProfile( std::string name ){
	Input_ProfileDataT* profile = GetProfile( name );
	if( profile != NULL ){
		ActiveProfile = profile;
		return true;
	}

	//if we get here it errored, so we set the default profile
	ActiveProfile = GetProfile( "default" );
	return false;
}
