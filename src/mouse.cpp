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
#include "display.h"

#include <SDL/SDL.h>
#include<boost/bind.hpp>

unsigned short* ptNumClicks;
bool* Buttons;
int x, y, oldx, oldy;
unsigned int ClickTimeout;
unsigned int ClickTime;
bool timerRunning;
bool possibleClick;
boost::function<void (bool) > ClickFunction;
Timer* timer;

void Mouse_Init(Display* d) {
	Buttons = new bool[3];
	x = 0;
	y = 0;
	ptNumClicks = new unsigned short(0);
	ClickTime = 0;
	ClickTimeout = 500;

	timer = new Timer(ClickTimeout);
	timer->SetFunction(boost::bind<void>(Mouse_StopTimer, true));
	ClickFunction = boost::bind<void>(&Display::OnMouseClick, d, ptNumClicks, _1);

	Mouse_SetState();
}

void Mouse_Die() {
	delete [] Buttons;
}

void Mouse_SetState() {
	oldx = x;
	oldy = y;

	unsigned int value = SDL_GetMouseState(&x, &y);
	Buttons[0] = value & SDL_BUTTON(1);
	Buttons[1] = value & SDL_BUTTON(2);
}

void Mouse_SetButtonState() {
	unsigned int value = SDL_GetMouseState(NULL, NULL);

	bool old = Buttons[0];

	//left
	Buttons[0] = value & SDL_BUTTON(1);

	//right
	Buttons[1] = value & SDL_BUTTON(2);

	if (old && !Buttons[0] && possibleClick) {
		//we have a click and not a press
		(*ptNumClicks)++;
		ClickFunction(false);
		possibleClick = false;

		if (timerRunning) {
			timer->Restart(false);
		} else {
			timerRunning = true;
			timer->Start();
		}
	} else if (Buttons[0]) {
		possibleClick = true;
	}
}

void Mouse_SetPosition() {
	//used for change
	oldx = x;
	oldy = y;

	SDL_GetMouseState(&x, &y);

	if (oldx != x || oldy != y) {
		possibleClick = false;

		if (timerRunning) {
			timerRunning = false;
			timer->Stop();
		}
	}
}

void Mouse_SetTimeout(int timeout) {
	ClickTimeout = timeout;
}

/**
 * Returns weather the button is up or down, if the key value is under 2
 * @param key the number of the key to get the button state, 0 = left, 1 = right
 * @returns true if the button is pressed;
 */
bool Mouse_GetButtonState(int key) {
	//out of bounds check
	if (key > 2)
		return false;

	return Buttons[key];

}

/**
 * This function will set the parameters to the mouse's value
 * @param retx A pointer to the value where you want the Mouse X position to be sent
 * @param rety A pointer to the value where you want the Mouse Y position to be sent
 */
void Mouse_GetPosition(int *retx, int *rety) {
	retx = &x;
	rety = &y;
}

int Mouse_GetX() {
	return x;
}

int Mouse_GetY() {
	return y;
}

int Mouse_GetChangeX() {
	return x - oldx;
}

int Mouse_GetChangeY() {
	return y - oldy;
}

Timer* Mouse_GetTimer() {
	return timer;
}

unsigned short Mouse_GetClicks() {
	return *ptNumClicks;
}

unsigned short* Mouse_GetClicksPtr() {
	return ptNumClicks;
}

/**
 * This function will stop the timer, calling the function if told to.
 * @param call a boolean telling weather or not the ClickFunction should be called
 */
void Mouse_StopTimer(bool call) {
	if (call) {
		ClickFunction(true);
	}

	*ptNumClicks = 0;
	timerRunning = false;
	timer->Stop();
}
