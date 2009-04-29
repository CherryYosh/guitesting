/*   This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	long with this program.  If not, see <http://www.gnu.org/licenses/>

 * 	Copyright 2008,2009 James Brandon Stevenson
 */

/*   James Stevenson 09/27/08
 *******************************************************************
 * A full function, SDL input class including mouse support
 * A action is bound before a key is bound to that action, including
 * a optional mod key and default actions if the key is not bound.
 * multipul keys may be bound to a single action without fear
 * key may be rebound without need to restart, actions however cant
 * be changed without modifying the source
 ******************************************************************
 * TODO: add unicode controler support, along with support for the
 * odditys found in some key boards / OS's (no ctrls in OSX, ) also
 * support for additional mouse buttons..
 */

#include "input.h"

#include "engine.h"
#include "display.h"

struct Input_ActionDataT{
	std::string Name; //the name of the action (IE: MOVEUP)
	bool UseOnce; //only handle once per press?
	bool Handled; //has it been handled yet?
	short Count; //number of keys being pressed
	boost::function<void()> Function;
};

struct Input_KeyDataT{
	SDLKey Key; //the key
	SDLMod Mod;
	Input_ActionDataT *Action;
};

struct Input_ProfileDataT{
	std::string Name;
	std::vector<Input_ActionDataT*> Actions;
	std::vector<Input_KeyDataT*> Keys;
	boost::function<void (SDL_keysym)> DefaultFunction;
};

Input::Input() : System(){
	input = this;

	//create a few empty profiles
	Input_ProfileDataT* temp = new Input_ProfileDataT;
	temp->Name = "default";
	temp->DefaultFunction = NULL;
	Profiles.push_back( temp );

	temp = new Input_ProfileDataT;
	temp->Name = "typing";
	temp->DefaultFunction = std::bind1st( std::mem_fun(&Display::OnKeyPress), display );
	Profiles.push_back( temp );

	SetProfile( "default" );
}

Input::~Input(){
	Profiles.clear();
	delete ActiveProfile;
	input = NULL;
}

/*
 * key events are polled and processed
 */
void Input::Start(){
	SDL_Event keyevent;

	ProcessInput();

	while( SDL_PollEvent( &keyevent ) ){
		switch( keyevent.type ){
			case SDL_KEYDOWN:
				ProcessKey( true, keyevent.key.keysym );
				break;
			case SDL_KEYUP:
				ProcessKey( false, keyevent.key.keysym );
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				display->OnMouseButtonChange();
				break;
			case SDL_MOUSEMOTION:
				display->OnMouseMotion();
				break;
			case SDL_ACTIVEEVENT:
				break;
			case SDL_VIDEOEXPOSE:
				break;
			case SDL_VIDEORESIZE:
				display->Resize( keyevent.resize.w, keyevent.resize.h );
				break;
			case SDL_QUIT:
				Engine::engine->Quit();
				return;
			default:
#ifdef _DEBUG_
				printf( "ERROR: Unkown sdl event type %i presented to Input\n", keyevent.type );
#endif
				break;
		}
	}
}


void Input::BindAction( std::string profile, std::string action, bool useOnce, boost::function<void()> function){
	Input_ActionDataT *newAction = new Input_ActionDataT;

	newAction->Name = action;
	newAction->Count = 0;
	newAction->UseOnce = useOnce;
	newAction->Handled = false;
	newAction->Function = function;

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
	Input_ActionDataT* a = NULL;

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

void Input::ProcessKey( bool pressed, SDL_keysym sym ){
	if( ActiveProfile == NULL ){
		return;
	}

	std::vector<Input_KeyDataT*> keys = ActiveProfile->Keys;
	Input_ActionDataT* action;

	size_t size = keys.size();
	for( unsigned int i = 0; i < size; i++ ){
		//				this allows use to have caps / numlock on while typeing
		if( keys[i]->Key == sym.sym && ( sym.mod & ~( KMOD_NUM | KMOD_CAPS ) ) == keys[i]->Mod ){
			action = keys[i]->Action;

			if( pressed )
				action->Count++;
			else{
				if( action->Count > 0 ) //wtf happens to make it == 0 when a key is still down??
					action->Count--;
				//reset the handeld flag if no keys are down
				if( action->Count == 0 && action->UseOnce )
					action->Handled = false;
			}
			//auidios..
			return;
		}
	}

	//else we do the default action
	if( pressed && ActiveProfile->DefaultFunction != NULL ){
		ActiveProfile->DefaultFunction(sym);
	}
}

void Input::ProcessInput(){
	std::vector<Input_ActionDataT*> actions = ActiveProfile->Actions;
	Input_ActionDataT* action;

	size_t size = actions.size();
	for( unsigned int i = 0; i < size; i++ ){
		action = actions[i];

		if( action->Count > 0 && !action->Handled ){
			action->Function();

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
