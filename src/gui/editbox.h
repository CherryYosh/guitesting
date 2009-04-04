#ifndef EDITBOX_H
#define EDITBOX_H

//#include "control.h"
#include "textbox.h"

class Editbox : public Textbox{
public:
	Editbox( std::string, int, int );
	~Editbox();
	void Render();
	bool HitTest( int, int );
	void onKeyPress( int, int );
	void onKeyRelease( int, int );
	void onMousePress( int );
	void onMouseRelease( int );
private:
protected:
};

#endif
