#ifndef MOUSE_H
#define MOUSE_H

#include <SDL/SDL.h>
#include <boost/thread/mutex.hpp>

//the mouse class for inpu
class Mouse{
public:
	Mouse();
	~Mouse();
	void SetButtonState();
	void SetMousePosition(); 
	void SetClickTimeout( int timeout );
	void SetClickValue( bool *value );

	bool GetButtonState( int key );
	void GetMousePosition( int *x, int *y );
	
	int GetX();
	int GetY();
	int GetChangeX();
	int GetChangeY();

private:
	bool *button; //is button x down?
	int x, y, oldx, oldy;
	unsigned int pressClicks; //the time the mouse was pressed, used for clicking
	unsigned int clickTimeout;

	int nClicks;

	boost::mutex mouse_mutex;
};

#endif
