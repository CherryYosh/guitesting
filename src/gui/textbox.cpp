#include "textbox.h"

#include "../fontmgr.h"

Textbox::Textbox( Window* p, std::string t, int x, int y ) : Control(p, t,x,y){
	TextWidth = 0;
	TextHeight = 0;
	Multiline = false;
	lines.push_back( "" );
	BottomLine = 0;
	CaretPos = 0;
	CaretLine = 0;
	font = 0;
	FlashCaret = false;
	ShowingCaret = false;
	Editable = false;
	NumCharacters = 0;
	MaxCharacters = 255;
	NumLines = 1;
	MaxLines = 8;
	Attributes |= CTRL_INPUT;
}

Textbox::~Textbox(){
	//nothing
}

void Textbox::Render(){
	//NOTE: We do nothing here, becouse we only want to draw text
}

void Textbox::RenderText(){
/*
	short size = lines.size();
	short line = BottomLine;
	for( short i = TextHeight; i > 0; i-- ){
		if( line >= 0 )
			FontMgr_glDrawText( font, x, y + ( FontMgr_GetLineHeight(font) * i), _TextShader, lines[line--].c_str() );
		else
			return;
	}
*/
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

void Textbox::SetFont( unsigned char f ){
	font = f;
	TextHeight = Height / FontMgr_GetLineHeight( font );
}

void Textbox::SetWidth( unsigned short w){
	Width = w;
	TextWidth = Width;
}

void Textbox::SetHeight( unsigned short h){
	Height = h;
	TextHeight = Height / FontMgr_GetLineHeight( font );
}
