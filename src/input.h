#ifndef INPUT_H
#define INPUT_H

#include <SDL/SDL.h>
#include <vector>
#include <string>

#include "system.h"


struct Input_ActionDataT{
	std::string Name; //the name of the action (IE: MOVEUP)
	bool UseOnce; //only handle once per press?
	bool Handled; //has it been handled yet?
	short Count; //number of keys being pressed
	short id; //the ID of the engine 
	int FunctionID; //the ID of the function to be called (will used by the target class
	void *Parameters; //the parameters to be passed to the function	
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
};


class Input : public System {
public:
	Input();
	~Input();

	void Start();
	
	void BindAction( std::string, short, std::string, bool, int, void* ); 
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
