#ifndef BUTTON_H
#define BUTTON_H

#include "control.h"

class Button : public Control
{
public:
	Button( std::string t, int x, int y );
	~Button();

	bool HitTest( int mouseX, int mouseY );
	void OnMousePress( int button );
protected:
private:
};

#endif
