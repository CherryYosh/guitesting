#include "label.h"
#include "../fontmgr.h"

Label::Label( std::string t, int x, int y ) : Control(t,x,y){
	TextWidth = Width;
	TextHeight = Height / FontMgr_GetLineHeight( font ); 
	Multiline = false;
	
	//FontString s;
	//s.font = 0;
	//s.Start = 0;
	//s.Length = 0;
	//s.Size = 0;
	//lines.push_back( s );
	
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

Label::~Label(){
	//nothing
}

void Label::RenderText(){
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

void Label::onMousePress( int button ){
	//needed?
}

void Label::onMouseRelease( int button ){
	//needed?
}

void Label::onKeyPress( unsigned short unicode ){ 
}

void Label::onKeyRelease( int key, int mod ){
	//needed?
}

void Label::SetFont( unsigned char f ){
	font = f;
	TextHeight = Height / FontMgr_GetLineHeight( font );
}

void Label::SetWidth( unsigned short w){
	Width = w;
	TextWidth = Width;
}

void Label::SetHeight( unsigned short h){
	Height = h;
	TextHeight = Height / FontMgr_GetLineHeight( font );
}
