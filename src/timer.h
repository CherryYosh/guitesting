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
/*********************
 * A simple FPS timer
 *
 * Known bugs:
 * 	fps not caling right
 *
 * James Brandon Stevenson @ 10/05/2008
*/

#ifndef TIMER_H
#define TIMER_H

#include <SDL/SDL.h>

#include <boost/function.hpp>

class Timer{
public:
	Timer();
	~Timer();

	bool Start();
	void Stop();
	void Restart(bool=true);
	void Tick();
	void Step();

	void Update();
	void Update(unsigned int);

	void SetFunction( boost::function<void()> );
	void SetRuntime( unsigned int );

	unsigned int* GetTicksPtr();
	unsigned int* GetStepsPtr();
	unsigned int GetTicks();
	unsigned int GetSteps();

private:
	void RunCommand();

	unsigned int CurTicks;
	unsigned int StopTicks;
	unsigned int RunTime;
	unsigned int Ticks;
	unsigned int Steps;

	bool Running;

	boost::function<void()> Function;
};
#endif
