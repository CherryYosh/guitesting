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
#ifndef TIMERPOOL_H
#define TIMERPOLL_H

#include "timer.h"
#include <vector>

struct timerpool_Data{
	Timer* timer;
	bool tick; //do we tick on update
	bool step; //do we step on update
};

class timerpool{
public:
	timerpool();
	~timerpool();

	void AddTimer( Timer*, bool, bool );
	void Update();
private:
	std::vector< timerpool_Data > timers;
};

#endif
