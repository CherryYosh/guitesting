#include <GL/glew.h>
#include "label.h"
#include "../fontmgr.h"

struct LABEL_VBOVertex{
	float x, y;
	float s,t;
	float r,g,b,a;
};

Label::Label( std::string t, int x, int y ) : Control(t,x,y){
	TextWidth = Width;
	TextHeight = Height / FontMgr_GetLineHeight( font ); 
	Multiline = false;
	
	BottomLine = 0;
	CaretPos = 0;
	CaretLine = 0;
	font = 0;
	FlashCaret = false;
	ShowingCaret = false;
	Editable = false;
	NumCharacters = 0;
	MaxCharacters = 100;
	NumLines = 1;
	MaxLines = 8;

	TextPosition = 0;
	TextLength = 0;

	Attributes |= CTRL_INPUT;
}

Label::~Label(){
	//nothing
}

void Label::RenderText( int vert, int text, int color ){
	size_t size = lines.size();
	for( unsigned int i = 0; i < size; i++ ){
		glVertexAttribPointer( vert, 2, GL_FLOAT, GL_FALSE, sizeof(LABEL_VBOVertex), (GLvoid*)(lines[i].Start) );
		glVertexAttribPointer( text, 2, GL_FLOAT, GL_FALSE, sizeof(LABEL_VBOVertex), (GLvoid*)(lines[i].Start + 2 * sizeof(float)) );
		glVertexAttribPointer( color, 4, GL_FLOAT, GL_FALSE, sizeof(LABEL_VBOVertex), (GLvoid*)(lines[i].Start + 4 * sizeof(float)) );

		glDrawArrays( GL_QUADS, 0, lines[i].Text.size() * 4 );
	}
}

void Label::AddStringsToVBO(){
	glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
	GLint size; glGetBufferParameteriv( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size );

	float* ptr = (float*)glMapBuffer( GL_ARRAY_BUFFER, GL_READ_ONLY );

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
}

void Label::ReplaceCharVBO( FontChar c ){
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
	float* ptr = (float*)glMapBufferRange( GL_ARRAY_BUFFER, pos, 4 * sizeof( LABEL_VBOVertex), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT );

	if( ptr != NULL ){
		memcpy( ptr, verts, 4 * sizeof( LABEL_VBOVertex ) );
		
		glUnmapBuffer( GL_ARRAY_BUFFER );
		glFlushMappedBufferRange( GL_ARRAY_BUFFER, 0,  4 * sizeof( LABEL_VBOVertex ) );
	} else {
		printf( "Error could not update character! %i\n", glGetError() );	
	}
	
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Label::UpdateVBO(){
	if( TextLength == 0 || TextPosition == 0 ){
		printf( "%i %i\n", TextLength, TextPosition );
		return;
	}


	size_t numLines = lines.size();
	LABEL_VBOVertex* data = new LABEL_VBOVertex[ NumCharacters * 4 ];
	FontString line;
	unsigned int slot = 0;
	FontChar c;
	float vx,vx2,vy,vy2;	//vertex data, prevent redundant cals
	float vs,vs2,vt,vt2;
	float r,g,b,a;
	std::list<FontChar>::iterator it;
	
	for( unsigned int i = 0;i < numLines; i++ ){
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

	float* ptr = (float*)glMapBufferRange( GL_ARRAY_BUFFER, TextPosition, TextLength, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT );

	if( ptr != NULL ){
		FontString line;
		unsigned int pos = 0;
		for(unsigned int i = 0; i < numLines; i++){
			line = lines[i];
			if( line.Text.size() > 0 ){
				memcpy( &ptr[ line.Start - TextPosition ], &data[pos], (line.Text.size() * 4) * sizeof( LABEL_VBOVertex ));
				pos += (line.Text.size() * 4);
			}
		}

		glUnmapBuffer( GL_ARRAY_BUFFER );
		glFlushMappedBufferRange( GL_ARRAY_BUFFER, 0, TextLength );
	} else {
		printf( "ERROR: could not rebuild text vbo! %i\n", glGetError() );
	}

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	delete [] data;
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

void Label::Move( float cx, float cy ){
	x += cx;
	y += cy;

	UpdateVBO();
}
