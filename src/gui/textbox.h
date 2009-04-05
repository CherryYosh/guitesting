#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "control.h"

#include <string>
#include <vector>

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
	unsigned short twidth, theight;
	unsigned short caretPos, caretLine;
	unsigned short bottomLine; //the index of the bottome line
	bool multiline;
	bool flashCaret;
	bool showingCaret;
	bool editable;
	unsigned char font; 		//fixed at 0 right now
	std::vector<std::string> lines;
private:
};

#endif
