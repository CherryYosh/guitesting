#ifndef BUTTON_H
#define BUTTON_H

#include "control.h"

class Button : public Control
{
public:
	Button( std::string t, int x, int y );
	~Button();

	bool HitTest( int mouseX, int mouseY );
	void onMousePress( int button );
	char GetType();
protected:
private:
};

#endif
