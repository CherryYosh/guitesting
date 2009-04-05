#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "control.h"

#include <string>

class Textbox : public Control {
public:
	Textbox( std::string t, int x, int y, int width, int height );
	~Textbox();

	virtual void Render(); //we will do nothing here
	virtual void RenderText();

	virtual void onMousePress( int button );
	virtual void onMouseRelease( int button );

	virtual void onKeyPress( unsigned short unicode ); 
	virtual void onKeyRelease( int key, int mod );

	static Shader* _TextShader;
protected:
	unsigned int twidth, theight;
	bool multiline;
	std::string text;
	unsigned int caretPos;
	unsigned char font; //fixed at 0 right now
private:
};

#endif
