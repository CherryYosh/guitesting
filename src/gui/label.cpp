/*   This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	long with this program.  If not, see <http://www.gnu.org/licenses/>

 * 	Copyright 2008,2009 James Brandon Stevenson
 */
#include <GL/glew.h>
#include "label.h"
#include "../fontmgr.h"

struct LABEL_VBOVertex{
	float x, y;
	float s,t;
	float r,g,b,a;
};

Label::Label( Window* p, std::string t, int x, int y ) : Control(p,t,x,y){
	TextWidth = Width;
	Multiline = false;

	BottomLine = 0;
	CaretPos = 0;
	CaretLine = 0;
	FlashCaret = false;
	ShowingCaret = false;
	Editable = false;
	NumCharacters = 0;
	MaxCharacters = 100;
	NumLines = 0;
	MaxLines = 8;

	TextPosition = 0;
	TextLength = 0;

	Attributes |= CTRL_INPUT;
}

Label::~Label(){
	//nothing
}

//TODO: Find a better way
void Label::RenderText( int vert, int text, int color ){
	size_t size = lines.size();
	for( unsigned int i = 0; i < size; i++ ){
		glVertexAttribPointer( vert, 2, GL_FLOAT, GL_FALSE, sizeof(LABEL_VBOVertex), (GLvoid*)(lines[i].Start) );
		glVertexAttribPointer( text, 2, GL_FLOAT, GL_FALSE, sizeof(LABEL_VBOVertex), (GLvoid*)(lines[i].Start + 2 * sizeof(float)) );
		glVertexAttribPointer( color, 4, GL_FLOAT, GL_FALSE, sizeof(LABEL_VBOVertex), (GLvoid*)(lines[i].Start + 4 * sizeof(float)) );

		glDrawArrays( GL_QUADS, 0, lines[i].Text.size() * 4 );
	}
}

void Label::AddChar( FontChar c, bool update ){
	if( NumLines == 0 )
		NumLines++;

	FontString* line = &lines[CaretLine];

	if( line->Text.size() > MaxCharacters)
		return;

	if( TextWidth < line->Width + c.advance ){

		if( !Multiline )
			return;

		//quick check just to see if we only need to add it to a new line
		if( CaretPos == line->Text.size() ){
			//if we are at the last line, we need to add one
			if( NumLines - 1 == CaretLine ){
				if( NumLines < MaxLines )
				    NumLines++;
				else
					return; //we dont do anything , were at the end
			}
			CaretLine++;
			CaretPos = 0;
			line->Width += c.advance;
			//restart with the new line
			AddChar( c, update );
			return;
		}

		//otherwise we are insterting a character, and deleteing the last
		FontChar old = (FontChar)(*--line->Text.end());
		line->Width -= old.advance;
		line->Text.pop_back();

		std::list<FontChar>::iterator it;
		it = line->Text.begin();
		std::advance(it, CaretPos++);
		line->Text.insert( it, c );
		line->Width += c.advance;

		if( update )
		    RebuildVBOLine( *line );

		//check to see if we are at the last line, need to add a new one
		if( NumLines - 1 == CaretLine ){
				if( NumLines < MaxLines ){
				    NumLines++;
				} else {
					return;
				}
		}

		//now we pop the last char to a new line
		//store temp values
		unsigned int cp = CaretPos;

		CaretLine++;
		CaretPos = 0;

		AddChar( old, update );

		CaretLine--;
		CaretPos = cp;

		return;
	}

	//else we can just insert
	if( CaretPos == line->Text.size() ){
		line->Text.push_back( c );

		if( update )
		    ReplaceCharVBO( c );
	} else {
		std::list<FontChar>::iterator it;
		it = line->Text.begin();
		std::advance(it, CaretPos);
		line->Text.insert( it, c );

		if( update )
		    RebuildVBOLine( *line );
	}

	NumCharacters++;
	line->Width += c.advance;
	CaretPos++;
}


void Label::AppendString( unsigned int lineNum, unsigned int position, FontString string ){
	//first make sure there is really a need to do anything
	if( string.Text.size() == 0 )
		return;

	unsigned int tpos = CaretPos;
	unsigned int tline = CaretLine;

	CaretPos = position;
	CaretLine = lineNum;

	std::list<FontChar>::iterator it;
	for( it = string.Text.begin(); it != string.Text.end() ; it++ ){
		AddChar( *it, false );
	}

	CaretPos = tpos;
	CaretLine = tline;

	UpdateVBO();
}
void Label::AddStringsToVBO(){
/*
	//glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
	GLint size; glGetBufferParameteriv( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size );

	char* ptr = (char*)glMapBuffer( GL_ARRAY_BUFFER, GL_READ_ONLY );

	if( ptr != NULL ){
		TextPosition = size;
		TextLength = (( 4 * sizeof( LABEL_VBOVertex)) * ( MaxLines * MaxCharacters ));
		float* newData = new float[ size + TextLength ];
		memcpy( newData, ptr, size );
		glUnmapBuffer( GL_ARRAY_BUFFER );

		unsigned int position = size;
		FontString s;
		s.font = 0;
		s.Size = 0;
		s.y = Height;
		s.Width = 0;
		s.Height = FontMgr_GetLineHeight( 0 );

		//TODO, clean this up? make it so tha only a single memset is called
		for( unsigned int i = 0; i < MaxLines; i++ ){
			s.Start = position;
			lines.push_back( s );

			glGenVertexArrays( 1, &lines[i].vao );

			for( unsigned int j = 0; j < MaxCharacters; j++ ){
				//fill the vbo with 0's
				memset( &newData[position], 0, 4 * sizeof( LABEL_VBOVertex ));
				position += 4 * sizeof( LABEL_VBOVertex );
			}

			s.y -= s.Height;
		}

		glBufferData( GL_ARRAY_BUFFER, position, newData, GL_STREAM_DRAW );
		delete [] newData;
	} else {
		printf( "ERROR could not add strings to vbo! %i\n", glGetError() );
	}

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
 */
}

void Label::ReplaceCharVBO( FontChar c ){
/*
	LABEL_VBOVertex verts[4];
	float vx, vx2, vy, vy2;
	float vs, vs2, vt, vt2;

	vx = c.x + x + lines[CaretLine].Width;
	vx2= c.x + x + ( lines[CaretLine].Width + c.width );
	vy = c.y + y + lines[CaretLine].y;
	vy2= c.y + y + ( lines[CaretLine].y + c.height );

	vs = c.s;
	vs2= c.s2;
	vt = c.t;
	vt2= c.t2;

	//top left
	verts[0].x = vx;
	verts[0].y = vy;
	verts[0].s = vs;
	verts[0].t = vt;
	verts[0].r = c.r;
	verts[0].g = c.g;
	verts[0].b = c.b;
	verts[0].a = c.a;

	//top right
        verts[1].x = vx2;
        verts[1].y = vy;
        verts[1].s = vs2;
        verts[1].t = vt;
        verts[1].r = c.r;
        verts[1].g = c.g;
        verts[1].b = c.b;
        verts[1].a = c.a;

	//bottom right
        verts[2].x = vx2;
        verts[2].y = vy2;
        verts[2].s = vs2;
        verts[2].t = vt2;
        verts[2].r = c.r;
        verts[2].g = c.g;
        verts[2].b = c.b;
        verts[2].a = c.a;

	//bottom left
        verts[3].x = vx;
        verts[3].y = vy2;
        verts[3].s = vs;
        verts[3].t = vt2;
        verts[3].r = c.r;
        verts[3].g = c.g;
        verts[3].b = c.b;
        verts[3].a = c.a;

	//This will replace the character at the CURRENT carret pos
	unsigned int pos = lines[CaretLine].Start + ( CaretPos * 4 * sizeof( LABEL_VBOVertex ) );

	glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
	char* ptr = (char*)glMapBufferRange( GL_ARRAY_BUFFER, pos, 4 * sizeof( LABEL_VBOVertex), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT );

	if( ptr != NULL ){
		memcpy( ptr, verts, 4 * sizeof( LABEL_VBOVertex ) );

		glFlushMappedBufferRange( GL_ARRAY_BUFFER, 0,  4 * sizeof( LABEL_VBOVertex ) );
		glUnmapBuffer( GL_ARRAY_BUFFER );
	} else {
		printf( "Error could not update character! %i\n", glGetError() );
	}

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
 */
}

void Label::RebuildVBOLine( FontString s ){
/*
	unsigned int slot = 0;
	float vx,vx2,vy,vy2;    //vertex data, prevent redundant cals
	float vs,vs2,vt,vt2;

	FontChar c;
	s.Width = 0;
	std::list<FontChar>::iterator it;
	LABEL_VBOVertex* data = new LABEL_VBOVertex[ s.Text.size() * 4 ];

	for( it = s.Text.begin(); it != s.Text.end(); it++ ){
		c = *it;

		vx = c.x + x + s.Width;
		vx2= c.x + x + ( s.Width + c.width );
		vy = c.y + y + s.y;
		vy2= c.y + y + ( s.y + c.height );

		s.Width += c.advance;

		vs = c.s;
		vs2= c.s2;
		vt = c.t;
		vt2= c.t2;

		//top left
		data[slot+0].x = vx;
		data[slot+0].y = vy;
		data[slot+0].s = vs;
		data[slot+0].t = vt;
		data[slot+0].r = c.r;
		data[slot+0].g = c.g;
		data[slot+0].b = c.b;
		data[slot+0].a = c.a;

		//top right
		data[slot+1].x = vx2;
		data[slot+1].y = vy;
		data[slot+1].s = vs2;
		data[slot+1].t = vt;
		data[slot+1].r = c.r;
		data[slot+1].g = c.g;
		data[slot+1].b = c.b;
		data[slot+1].a = c.a;

		//bottom right
		data[slot+2].x = vx2;
		data[slot+2].y = vy2;
		data[slot+2].s = vs2;
		data[slot+2].t = vt2;
		data[slot+2].r = c.r;
		data[slot+2].g = c.g;
		data[slot+2].b = c.b;
		data[slot+2].a = c.a;

		//bottom left
		data[slot+3].x = vx;
		data[slot+3].y = vy2;
		data[slot+3].s = vs;
		data[slot+3].t = vt2;
		data[slot+3].r = c.r;
		data[slot+3].g = c.g;
		data[slot+3].b = c.b;
		data[slot+3].a = c.a;

		slot += 4;
	}

        glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
	unsigned int length =  (s.Text.size() * 4) * sizeof( LABEL_VBOVertex );

	char* ptr = (char*)glMapBufferRange( GL_ARRAY_BUFFER, s.Start, length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT );

        if( ptr != NULL ){
                memcpy( ptr, data, length );

		glFlushMappedBufferRange( GL_ARRAY_BUFFER, 0, length );
		glUnmapBuffer( GL_ARRAY_BUFFER );
	} else {
                printf( "ERROR: could not rebuild text line vbo! %i\n", glGetError() );
        }

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        delete [] data;
*/
 }

void Label::UpdateVBO(){
/*
	if( TextLength == 0 || TextPosition == 0 || NumLines == 0 ){
		return;
	}

	size_t numLines = lines.size();
	LABEL_VBOVertex* data = new LABEL_VBOVertex[ NumCharacters * 4 ];
	FontString line;

	unsigned int slot = 0;
	float vx,vx2,vy,vy2;	//vertex data, prevent redundant cals
	float vs,vs2,vt,vt2;
	float r,g,b,a;

	FontChar c;
	std::list<FontChar>::iterator it;

	for( unsigned int i = 0; i < numLines; i++ ){
		line = lines[i];
		line.Width = 0;
		for( it = line.Text.begin(); it != line.Text.end(); it++ ){
			c = *it;

			//note sure if this helps much,
			//but does make the code look cleaner :P
			//and im hoping this will allow the compiler to make it into SSA
			vx = c.x + x + line.Width;
			vx2= c.x + x + ( line.Width + c.width );
			vy = c.y + y + line.y;
			vy2= c.y + y + ( line.y + c.height );

			line.Width += c.advance;

			vs = c.s;
			vs2= c.s2;
			vt = c.t;
			vt2= c.t2;

			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;

			//top left
			data[slot+0].x = vx;
			data[slot+0].y = vy;
			data[slot+0].s = vs;
			data[slot+0].t = vt;
			data[slot+0].r = r;
			data[slot+0].g = g;
			data[slot+0].b = b;
			data[slot+0].a = a;

			//top right
			data[slot+1].x = vx2;
                        data[slot+1].y = vy;
                        data[slot+1].s = vs2;
                        data[slot+1].t = vt;
                        data[slot+1].r = r;
                        data[slot+1].g = g;
                        data[slot+1].b = b;
                        data[slot+1].a = a;

			//bottom right
                 	data[slot+2].x = vx2;
                        data[slot+2].y = vy2;
                        data[slot+2].s = vs2;
                        data[slot+2].t = vt2;
                        data[slot+2].r = r;
                        data[slot+2].g = g;
                        data[slot+2].b = b;
                        data[slot+2].a = a;

			//bottom left
			data[slot+3].x = vx;
                        data[slot+3].y = vy2;
                        data[slot+3].s = vs;
                        data[slot+3].t = vt2;
                        data[slot+3].r = r;
                        data[slot+3].g = g;
                        data[slot+3].b = b;
                        data[slot+3].a = a;

			slot += 4;
		}
	}

	glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );

	char* ptr = (char*)glMapBufferRange( GL_ARRAY_BUFFER, TextPosition, TextLength, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT );

	if( ptr != NULL ){
		unsigned int pos = 0;
		unsigned int size = 0;
		for(unsigned int i = 0; i < numLines; i++){
			line = lines[i];
			if( line.Text.size() > 0 ){
				size = (line.Text.size() * 4);
				memcpy( &ptr[ line.Start - TextPosition ], &data[pos], size * sizeof( LABEL_VBOVertex ));
				pos += size;
			}
		}

		glFlushMappedBufferRange( GL_ARRAY_BUFFER, 0, TextLength );
		glUnmapBuffer( GL_ARRAY_BUFFER );
	} else {
		printf( "ERROR: could not rebuild text vbo! %i\n", glGetError() );
	}

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	delete [] data;
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

void Label::SetWidth( unsigned short w){
	Width = w;
	TextWidth = Width;
}

void Label::SetHeight( unsigned short h){
	Height = h;
}

void Label::Move( float cx, float cy ){
	x += cx;
	y += cy;

	UpdateVBO();
}
