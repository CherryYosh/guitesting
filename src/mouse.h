#ifndef MOUSE_H
#define MOUSE_H

void Mouse_Init();
void Mouse_Die();

unsigned char Mouse_SetState();
bool Mouse_SetButtonState();
bool Mouse_SetPosition(); 
void Mouse_SetClickTimeout( unsigned int );
void Mouse_SetClickValue( bool* );

bool Mouse_GetButtonState( int );
void Mouse_GetMousePosition( unsigned int*, unsigned int* );
	
unsigned int Mouse_GetX();
unsigned int Mouse_GetY();
unsigned int Mouse_GetChangeX();
unsigned int Mouse_GetChangeY();

#endif
