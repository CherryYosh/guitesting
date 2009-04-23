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
#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef _DEBUG_
#include <stdio.h>
#endif

class Input;
class Display;

class System{
public:
	System();
	virtual ~System();

	virtual void Start();

	static Input* input;
	static Display* display;
protected:
private:
};

#endif
