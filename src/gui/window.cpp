/* The window class
 * 	Binds several Controls together, if one moves they all move
 *
 * TODO:
 * 	Fix close so that the window will stay in memory for X seconds before being handled by GC
 *
 * James Brandon Stevenson 
 *
 */
/*
#include "window.h"

Window::Window(){
	list = new tControlList; 
	list->control = NULL; 
	list->next = NULL;
}

Window::~Window(){
	tControlList *temp, *next;

	temp = list;

	while( temp ){
		next = temp->next;
		
		delete temp;
		temp = next;
	}
}

void Window::AddChild( Control *child ){
	if( list == NULL ){
		list->control = child;
		list->next = NULL;
		return;
	}

	tControlList *newList = new tControlList;
	newList->control = child;
	newList->next = NULL;

	//move to the end of the list and add the child
	tControlList *temp = list;	
	
	while( temp->next )
		temp = temp->next;
	
	temp->next = newList;
}

void Window::Move( int xChange, int yChange ){
	tControlList *temp = list;

	while( temp ){
		temp->control->Move( xChange, yChange );
		temp = temp->next;
	}
}

void Window::Close(){
	/*TODO
	 * Allow saving, so upon reopening the buttons do not need to be reloaded
	 * will need a GC to truly delete the window after x seconds..
	 */
//	delete this;
//}
//*/
