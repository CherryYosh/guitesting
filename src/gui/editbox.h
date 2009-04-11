#ifndef EDITBOX_H
#define EDITBOX_H

//#include "control.h"
#include "label.h"

class Editbox : public Label{
public:
	Editbox( std::string, int, int );
	~Editbox();
	//void Render();
	bool HitTest( int, int );
	void OnKeyPress( unsigned short );
	void OnKeyRelease( int, int );
	void OnMousePress( int );
	void OnMouseRelease( int );
private:
protected:
};

#endif
