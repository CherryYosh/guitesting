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

timerpool::timerpool(){
}

timerpool::~timerpool(){
	timers.clear();
}

void timerpool::AddTimer( Timer* timer, bool tick = false, bool step = false ){
	timerpool_Data data;
	data.timer = timer;
	data.tick = tick;
	data.step = step;

	timers.push_back( data );
}

void timerpool::Update(){
	size_t size = timers.size();
	timerpool_Data data;
	for( unsigned int i = 0; i < size; i++ ){
		data = timers[i];

		if( data.timer == NULL )
			break;

		if( data.tick )
			data.timer->Tick();
		if( data.step )
			data.timer->Step();
		data.timer->Update();
	}
}
