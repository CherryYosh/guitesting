#include "textbox.h"

#include "../fontmgr.h"

Textbox::Textbox( std::string t, int x, int y, int width, int height ) : Control(t,x,y){
	twidth = width;
	theight = height;
	multiline = false;
	text = ""; 
	caretPos = 0;
	font = 0;
	attributes |= CTRL_INPUT;
}

Textbox::~Textbox(){
	//nothing
}

void Textbox::Render(){
	//NOTE: We do nothing here, becouse we only want to draw text
}

void Textbox::RenderText(){
	FontMgr_glDrawText( font, x, y, _TextShader, text.c_str() ); 
}

void Textbox::onMousePress( int button ){
	//needed?
}

void Textbox::onMouseRelease( int button ){
	//needed?
}

void Textbox::onKeyPress( unsigned short unicode ){ 
}

void Textbox::onKeyRelease( int key, int mod ){
	//needed?
}
