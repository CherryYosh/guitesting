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
#include "timerpool.h"

struct timerpool_Data {
	Timer* timer;
	bool tick; //do we tick on update
	bool step; //do we step on update
};

timerpool::timerpool(){
}

timerpool::~timerpool(){
	timers.clear();
}

void timerpool::AddTimer( Timer* timer, bool tick, bool step ){
	if( timer == NULL )
		return;

	timerpool_Data data;
	data.timer = timer;
	data.tick = tick;
	data.step = step;

	timers.push_back( data );
}

void timerpool::Update(){
	timerpool_Data data;
	unsigned int ticks = SDL_GetTicks();
	size_t size = timers.size();
	for( unsigned int i = 0; i < size; i++ ){
		data = timers[i];

		if( data.tick )
			data.timer->Tick();
		if( data.step )
			data.timer->Step();

		data.timer->Update(ticks);
	}
}
