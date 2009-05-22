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
#ifdef _DEBUG_
	#include <stdio.h>
#endif

#include <boost/bind.hpp>

#include "engine.h"
#include "input.h"
#include "display.h"
#include "thememgr.h"

#include "lua/luabase.h"

Engine* Engine::engine;

Engine::Engine(){
	engine = this;

	lua = new LUABase();
	lua->Init();

	display = new Display();

	Theme theme;
	theme.Init();
	theme.LoadTheme("scripts/xmlParse.lua");
	
	display->Temp();

	input = new Input();

	input->BindAction( "default", "QUIT", true, boost::bind<void>( &Engine::Quit, Engine::engine ) );
	input->BindKey( "default", SDLK_q, KMOD_LCTRL, "QUIT" );
	input->BindKey( "default", SDLK_q, KMOD_NONE, "QUIT" );

	input->BindAction( "typing", "QUIT", true, boost::bind<void>( &Engine::Quit, Engine::engine ) );
	input->BindKey( "typing", SDLK_q, KMOD_LCTRL, "QUIT" );
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
	}
}

void Engine::Start(){
	Run();

	delete this;
}
