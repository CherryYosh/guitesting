#ifndef INPUT_H
#define INPUT_H

#include <SDL/SDL.h>
#include <vector>
#include <string>
#include <boost/function.hpp>

#include "system.h"


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


class Input : public System {
public:
	Input();
	~Input();

	void Start();
	
	void BindAction( std::string, std::string, bool, boost::function<void()> ); 
	void BindKey( std::string, SDLKey, SDLMod, std::string );
	void ProcessKey( bool, SDL_keysym );//SDLKey, SDLMod );
	void ProcessInput();

	Input_ProfileDataT* GetProfile( std::string );
	bool SetProfile( std::string name );
private:
	std::vector<Input_ProfileDataT*> Profiles;
	Input_ProfileDataT *ActiveProfile;
};

#endif
