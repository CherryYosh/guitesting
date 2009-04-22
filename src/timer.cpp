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
#include "timer.h"

Timer::Timer(){
	Running = false;
	CurTicks = 0;
	StopTicks = 0;
	RunTime = 1000;
	Ticks = 0;
	Steps = 0;
}

Timer::~Timer(){
}

bool Timer::Start(){
	//a check to make sure its not started
	if( Running )
		return false;

	//set up the timer
	Running = true;
	CurTicks = SDL_GetTicks();
	StopTicks = CurTicks + RunTime;
	
	//zero out the data
	Ticks = 0;
	Steps = 0;

	return true;
}

void Timer::Stop(){
	Running = false;
}

void Timer::Restart( bool reset ){
	StopTicks = SDL_GetTicks() + RunTime;
	
	if( reset ){
		Ticks = 0;
		Steps = 0;
	}
}

void Timer::Update(){
	if( !Running )
		return;

	CurTicks = SDL_GetTicks();

	if( CurTicks >= StopTicks ){
		Running = false; //NOTE: This has to be aboce RunCommand, so the command can correctly restart if it needs to
		RunCommand();
	}
}

void Timer::Tick(){
	Ticks++;
}

void Timer::Step(){
	Steps++;
}

void Timer::SetFunction( boost::function<void()> function){
	Function = function;
}

void Timer::RunCommand(){
	Function();
}

void Timer::SetRuntime( unsigned int time ){
	//NOTE: this doesnt need to be locked, not used by the timer thread
	RunTime = time;
}

unsigned int* Timer::GetTicksPtr(){
	return &Ticks;
}

unsigned int* Timer::GetStepsPtr(){
	return &Steps;
}

unsigned int Timer::GetTicks(){
	return Ticks;
}

unsigned int Timer::GetSteps(){
	return Steps;
}
