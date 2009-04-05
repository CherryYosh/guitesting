#include "textbox.h"

#include "../fontmgr.h"

Textbox::Textbox( std::string t, int x, int y, int width, int height ) : Control(t,x,y){
	twidth = width; //width in pixels
	theight = height; //height in lines
	multiline = false;
	lines.push_back( "" );
	bottomLine = 0;
	caretPos = 0;
	caretLine = 0;
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
	short size = lines.size();
	short line = bottomLine;
	for( short i = theight; i > 0; i-- ){
		if( line >= 0 )
			FontMgr_glDrawText( font, x, y + ( FontMgr_GetLineHeight(font) * i), _TextShader, lines[line--].c_str() ); 
		else
			return;
	}
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
