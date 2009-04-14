#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "control.h"
#include "../fontmgr.h"

#include <string>
#include <vector>
#include <list>

struct FontChar{
	char c;		//the actual char
	float r,g,b,a;	//colors
	float s,t,s2,t2;//texcoords
	float x,y;	//vertex position
	float width,height;	//width of the texture
};
struct FontString{
	unsigned short font;		//the font id
	unsigned short Size;		//the number of characters
	unsigned int Start; 		//the first position in the VBO
	unsigned int Length;		//the length (in bytes) we occupy in the vbo
	float y;			//the y coord wont change inside of a string 
	unsigned int Width, Height;	//the current width of the string (used to find the x of the next char) and the height (needed?)
	std::list<FontChar> Text;
	unsigned int vao;
};

class Label : public Control {
public:
	Label( std::string t, int x, int y );
	~Label();

	virtual void RenderText( int, int, int );

	virtual void onMousePress( int button );
	virtual void onMouseRelease( int button );

	virtual void onKeyPress( unsigned short unicode ); 
	virtual void onKeyRelease( int key, int mod );

	virtual void SetFont( unsigned char f );
	virtual void SetWidth( unsigned short );
	virtual void SetHeight( unsigned short );

	virtual void UpdateVBO();
	virtual void AddStringsToVBO();
	virtual void ReplaceCharVBO( FontChar );
protected:
	unsigned short TextWidth, TextHeight;
	unsigned short CaretPos, CaretLine;
	unsigned short BottomLine; //the index of the bottome line
	unsigned short NumCharacters, NumLines;
	unsigned short MaxCharacters, MaxLines;
	unsigned int TextPosition, TextLength;
	bool Multiline;
	bool FlashCaret;
	bool ShowingCaret;
	bool Editable;
	unsigned char font; 		//fixed at 0 right now
	std::vector<FontString> lines;
private:
};

#endif
