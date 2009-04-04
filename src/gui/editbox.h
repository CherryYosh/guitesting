#ifndef EDITBOX_H
#define EDITBOX_H

#include "control.h"

class Editbox : public Control{
public:
	Editbox( std::string, int, int );
	~Editbox();
	bool HitTest( int, int );
	void onKeyPress( int, int );
	void onKeyRelease( int, int );
	void onMousePress( int );
	void onMouseRelease( int );
private:
protected:
	unsigned int tx, ty, twidth, theight; //texture stuff, are they needed?
	unsigned short caretPos, size;
	std::string text;
};

#endif
