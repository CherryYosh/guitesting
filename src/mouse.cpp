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
 * James Stevenson @ 09/30/08
 */

#include "mouse.h"
#include <SDL/SDL.h>

unsigned char NumClicks;
bool* Buttons;
unsigned int x, y, oldx, oldy;
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
	delete Buttons;
}

unsigned char Mouse_SetState(){
	
	unsigned char ret = 0;
	/*
	if(Mouse_SetButtonState())
		ret = 1;
	if( Mouse_SetPosition())
		ret = ret | 0x02;
	*/
	return ret;
}

bool Mouse_SetButtonState(){
	bool old[2];
	old[0] = Buttons[0];
	old[1] = Buttons[1];
	
	//left
	Buttons[0] =  ( SDL_GetMouseState( NULL, NULL ) & SDL_BUTTON(1) );
	
	//right
	Buttons[1] =  ( SDL_GetMouseState( NULL, NULL ) & SDL_BUTTON(2) );

	return !( old[1] == Buttons[0] && old[2] == Buttons[2] );
}

bool Mouse_SetPosition(){
	//used for change
	oldx = x;
	oldy = y;

	SDL_GetMouseState( (int*)&x, (int*)&y );
	
	return !(oldx == x && oldy == y);
}

void Mouse_SetTimeout(int timeout ){
	ClickTimeout = timeout;
}

bool Mouse_GetButtonState( int key ){
	//out of bounds check
	//TODO: remove magic number
	if( key > 2 )
		return false;

	return Buttons[key];

}

void Mouse_GetPosition( unsigned int *retx, unsigned int *rety ){
	retx = &x;
	rety = &y;
}

unsigned int Mouse_GetX(){
	return x;
}

unsigned int Mouse_GetY(){
	return y;
}

unsigned int Mouse_GetChangeX(){
	return x - oldx;
}
unsigned int Mouse_GetChangeY(){
	return y - oldy;
}

//int Mouse::GetClicks(){
//	return clicks;
//}
