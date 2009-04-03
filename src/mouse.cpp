/*******************************
 * A simple mouse class featuring
 * a adjustable click time
 *
 * TODO: double clicks, more then 3
 * button support, isure proper touch
 * pad support... 
 *
 *
 * KNOWN BUGS:
 * 	none.. ^^
 *
 *
 * James Stevenson @ 09/30/08
 */

#include "mouse.h"
#include <stdio.h>

Mouse::Mouse(){	
	button = new bool[3]; //TODO: remove the magic number to suppror more buttons
	x = 0;
	y = 0;
	pressClicks = 0;
	clickTimeout = 1000; //1second
	SetButtonState();
}

Mouse::~Mouse(){
	delete button;
}

void Mouse::SetButtonState(){
	//left
	if( SDL_GetMouseState( NULL, NULL ) & SDL_BUTTON(1) )
		button[0] = true;
	else 
		button[0] = false;
	
	//right
	if( SDL_GetMouseState( NULL, NULL ) & SDL_BUTTON(2) )
	        button[1] = true;
	else 
	        button[1] = false;
}

//get the mouse pos from SDL, setting x and y
void Mouse::SetMousePosition(){
	//used for change
	oldx = x;
	oldy = y;

	SDL_GetMouseState( &x, &y );
}

void Mouse::SetClickTimeout(int timeout ){
	clickTimeout = timeout;
}

bool Mouse::GetButtonState( int key ){
	//out of bounds check
	//TODO: remove magic number
	if( key > 2 )
		return false;

	return button[key];

}

void Mouse::GetMousePosition( int *retx, int *rety ){
	retx = &x;
	rety = &y;
}

int Mouse::GetX(){
	return x;
}

int Mouse::GetY(){
	return y;
}

int Mouse::GetChangeX(){
	return x - oldx;
}
int Mouse::GetChangeY(){
	return y - oldy;
}

//int Mouse::GetClicks(){
//	return clicks;
//}
