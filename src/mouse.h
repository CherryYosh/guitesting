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
#ifndef MOUSE_H
#define MOUSE_H

#include "timer.h"

class Display;

void Mouse_Init(Display*);
void Mouse_Die();

void Mouse_SetState();
void Mouse_SetButtonState();
void Mouse_SetPosition();
void Mouse_SetClickTimeout(unsigned int);
void Mouse_SetClickValue(bool*);

bool Mouse_GetButtonState(int);
void Mouse_GetMousePosition(int*, int*);

int Mouse_GetX();
int Mouse_GetY();
int Mouse_GetChangeX();
int Mouse_GetChangeY();

Timer* Mouse_GetTimer();
unsigned short Mouse_GetClicks();
unsigned short* Mouse_GetClicksPtr();

void Mouse_StopTimer(bool = false);

#endif
