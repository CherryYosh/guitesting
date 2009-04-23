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
#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include <boost/function.hpp>

#include "system.h"

struct Input_ProfileDataT;

class Input : public System {
public:
	Input();
	~Input();

	void Start();

	void BindAction( std::string, std::string, bool, boost::function<void()> );
	void BindKey( std::string, SDLKey, SDLMod, std::string );
	void ProcessKey( bool, SDL_keysym );
	void ProcessInput();

	Input_ProfileDataT* GetProfile( std::string );
	bool SetProfile( std::string name );
private:
	std::vector<Input_ProfileDataT*> Profiles;
	Input_ProfileDataT *ActiveProfile;
};

#endif
