#ifndef MOUSE_H
#define MOUSE_H

void Mouse_Init();
void Mouse_Die();

void Mouse_SetState();
void Mouse_SetButtonState();
void Mouse_SetPosition();
void Mouse_SetClickTimeout( unsigned int );
void Mouse_SetClickValue( bool* );

bool Mouse_GetButtonState( int );
void Mouse_GetMousePosition( int*, int* );
	
int Mouse_GetX();
int Mouse_GetY();
int Mouse_GetChangeX();
int Mouse_GetChangeY();

#endif
