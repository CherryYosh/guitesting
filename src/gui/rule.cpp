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
//A base rule class used for things like borders, spacers seperators etc


#include "rule.h"

Rule::Rule(std::string t, Window* p, Control* c, float x, float y ) : Control(t,p,c,x,y) {
}

Rule::~Rule(){
}

bool Rule::HitTest( int mouseX, int mouseY ){
	if( mouseX > x && mouseY > y  &&
			mouseX < x + Width &&  mouseY < y + Height ){
		return true;
	}

	return false;
}
