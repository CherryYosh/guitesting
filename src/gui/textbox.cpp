#include "textbox.h"

#include "../fontmgr.h"

Textbox::Textbox( std::string t, int x, int y, int width, int height ) : Control(t,x,y){
	twidth = width; //width in pixels
	theight = 1;//height; //height in characters
	multiline = false;
	text = ""; 
	caretPos = 0;
	font = 0;
	flashCaret = false;
	showingCaret = false;
	editable = false;
	attributes |= CTRL_INPUT;
}

Textbox::~Textbox(){
	//nothing
}

void Textbox::Render(){
	//NOTE: We do nothing here, becouse we only want to draw text
}

void Textbox::RenderText(){
	FontMgr_glDrawText( font, x, y + FontMgr_GetLineHeight(font), _TextShader, text.c_str() ); 
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
