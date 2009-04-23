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
/*******************************
 * A simple mouse class featuring
 * a adjustable click time
 *
 * TODO: double clicks, more then 3
 * Buttons support, isure proper touch
 * pad support...
 *
 *
 * KNOWN BUGS:
 * 	none.. ^^
 *
 *
 * James Stevenson @ 04/17/09
 */

#include "mouse.h"
#include <SDL/SDL.h>

unsigned char NumClicks;
bool* Buttons;
int x, y, oldx, oldy;
unsigned int ClickTimeout;
unsigned int ClickTime;

void Mouse_Init(){
	Buttons = new bool[3];
	x = 0;
	y = 0;
	ClickTime = 0;
	ClickTimeout = 1000; //1second

	Mouse_SetState();
}

void Mouse_Die(){
	delete [] Buttons;
}

void Mouse_SetState(){
	oldx = x;
	oldy = y;

	unsigned int value = SDL_GetMouseState( &x, &y );
	Buttons[0] = value & SDL_BUTTON(1);
	Buttons[1] = value & SDL_BUTTON(2);
}

void Mouse_SetButtonState(){
	unsigned int value = SDL_GetMouseState( NULL, NULL );

	//left
	Buttons[0] = value & SDL_BUTTON(1);

	//right
	Buttons[1] = value & SDL_BUTTON(2);
}

void Mouse_SetPosition(){
	//used for change
	oldx = x;
	oldy = y;

	SDL_GetMouseState( &x, &y );
}

void Mouse_SetTimeout(int timeout ){
	ClickTimeout = timeout;
}

bool Mouse_GetButtonState( int key ){
	//out of bounds check
	if( key > 2 )
		return false;

	return Buttons[key];

}

void Mouse_GetPosition( int *retx, int *rety ){
	retx = &x;
	rety = &y;
}

int Mouse_GetX(){
	return x;
}

int Mouse_GetY(){
	return y;
}

int Mouse_GetChangeX(){
	return x - oldx;
}

int Mouse_GetChangeY(){
	return y - oldy;
}

