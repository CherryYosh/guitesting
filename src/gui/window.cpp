/* The window class
 * 	Binds several Controls together, if one moves they all move
 *
 * TODO:
 * 	Fix close so that the window will stay in memory for X seconds before being handled by GC
 *
 * James Brandon Stevenson 
 *
 */
#include <GL/glew.h>
#include "window.h"

Window::Window(){
	VertexPosition = 0;
	VertexLength = 0;
	VertexPositionIsSet = false;
}

Window::~Window(){
}

void Window::AddChild( Control *child, int depth, bool rebuild ){
	child->SetDepth( depth );
	Children.push_back( child );

	if( rebuild )
		RebuildVBO(); //rebuild the texture too
}

void Window::Move( float xChange, float yChange ){
	x += xChange;
	y += yChange;

	UpdateVBO();
}

void Window::Close(){
	/*TODO
	 * Allow saving, so upon reopening the buttons do not need to be reloaded
	 * will need a GC to truly delete the window after x seconds..
	 */
	delete this;
}

void Window::Render(){
}

bool Window::HitTest( int mx, int my ){
	//TODO: make this faster :P
	if( mx > x && mx < x + width &&
			my > y && my < y + height ){
		//now we test the controls
		//remove the windows position from the mouse click..
		//due to the fact that the children are based on the window's position
		mx -= x;
		my -= y;
		size_t size = Children.size();
		for( int i = 0; i < size; i++ ){
			if(Children[i]->HitTest(mx, my))
				return true;
		}

		
		return true;
	}
	return false;
}

void Window::UpdateVBO(){
	//NOTE: This is a lot like rebuild, only the end changes..
	
	size_t numChildren = Children.size();
	WINDOW_VBOVertex* data = new WINDOW_VBOVertex[ numChildren * 4 ];

	Control* child;
	unsigned int slot;
	float vx, vx2, vy, vy2; //the vertex values, prevent redundant calculations
	float vs, vs2, vt, vt2;
	for( unsigned int i = 0; i < numChildren; i++ ){
		//NOTE: I am hoping this code will be made into SSE :]
		
		slot = i * 4;
		child = Children[i];
		
		vx = x + child->x;
		vx2 = x + ( child->x + child->GetWidth() );
		vy = y + child->y;
		vy2 = y + ( child->y + child->GetHeight() );

		vs = child->s;
		vs2 = child->s2;
		vt = child->t;
		vt2 = child->t2;

		//top left
		data[slot+0].x = vx;
		data[slot+0].y = vy;
		data[slot+0].s = vs;
		data[slot+0].t = vt;

		//top right
		data[slot+1].x = vx2;
		data[slot+1].y = vy;
		data[slot+1].s = vs2;
		data[slot+1].t = vt;

		//bottom right
		data[slot+2].x = vx2;
		data[slot+2].y = vy2;
		data[slot+2].s = vs2;
		data[slot+2].t = vt2;
		
		//bottom left
		data[slot+3].x = vx;
		data[slot+3].y = vy2;
		data[slot+3].s = vs;
		data[slot+3].t = vt2;
	}

	glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
	
	GLint size;
	glGetBufferParameteriv( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size );

	void* ptr = glMapBufferRange( GL_ARRAY_BUFFER, VertexPosition, VertexLength, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT );

	if( ptr != NULL ){
		memcpy( ptr, data, VertexLength );
		//NOTE: Do a check here to make sure Unmap returns GL_TRUE :P
		glUnmapBuffer( GL_ARRAY_BUFFER );
		glFlushMappedBufferRange( GL_ARRAY_BUFFER_ARB, VertexPosition, VertexLength );
	} else {
		GLenum error = glGetError();
		printf( "Error: Could not update vbo! %i\n", error );
	}


	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	delete [] data;
}


void Window::RebuildVBO(){
	//alright so here we will clear out the old data (if there is any)
	//and replace it with new data, even if we have more data..
	//NOTE: This should only be called after adding or removing a child... Otherwise use Update
	size_t numChildren = Children.size();
        WINDOW_VBOVertex* data = new WINDOW_VBOVertex[ numChildren * 4 ];

        Control* child;
        unsigned int slot;
        float vx, vx2, vy, vy2; //the vertex values, prevent redundant calculations
        float vs, vs2, vt, vt2;
	for( unsigned int i = 0; i < numChildren; i++ ){
                //NOTE: I am hoping this code will be made into SSE :]

                slot = i * 4;
                child = Children[i];

                vx = x + child->x;
                vx2 = x + ( child->x + child->GetWidth() );
                vy = y + child->y;
                vy2 = y + ( child->y + child->GetHeight() );

                vs = child->s;
                vs2 = child->s2;
                vt = child->t;
                vt2 = child->t2;

		//top left
                data[slot+0].x = vx;
                data[slot+0].y = vy;
                data[slot+0].s = vs;
                data[slot+0].t = vt;

                //top right
                data[slot+1].x = vx2;
                data[slot+1].y = vy;
                data[slot+1].s = vs2;
                data[slot+1].t = vt;

                //bottom right
                data[slot+2].x = vx2;
                data[slot+2].y = vy2;
                data[slot+2].s = vs2;
                data[slot+2].t = vt2;

                //bottom left
                data[slot+3].x = vx;
                data[slot+3].y = vy2;
                data[slot+3].s = vs;
                data[slot+3].t = vt2;
        }

	unsigned int length = (numChildren * 4 * sizeof(WINDOW_VBOVertex)); 
	glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
	GLint size;
	glGetBufferParameteriv( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size );
	
	if( size == 0 ){
		glBufferData( GL_ARRAY_BUFFER, length, data, GL_STREAM_DRAW );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );

		VertexPositionIsSet = true;
		VertexLength = length;
		
		delete [] data;
		return;
	}

	//alright so we have to get the full data
	float* ptr = (float*)glMapBufferRange( GL_ARRAY_BUFFER, VertexPosition, VertexPositionIsSet ? VertexLength : size, GL_MAP_READ_BIT );
	
	float* newData = new float[ ( size + ( length - VertexLength ))  ];
	
	if( ptr != NULL ){
		//build the new data
		memcpy( newData, ptr, VertexPositionIsSet ? VertexPosition : (VertexPosition = size) );
		memcpy( &newData[ VertexPosition ], data, length );
		
		if( VertexPositionIsSet ) //if its set we need to copy everything after it
			memcpy( &newData[ VertexPosition + length ], &ptr[ VertexPosition + VertexLength  ], abs(size - (length + VertexPosition)) );

		glBufferData( GL_ARRAY_BUFFER, size + ( length - VertexLength ), newData, GL_STREAM_DRAW ); 
		VertexLength = length;
		VertexPositionIsSet = true;
	} else {
		GLenum error = glGetError();
		printf( "ERROR: Could not rebuild gui vbo! %i\n", error );
	}
	
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	delete [] data;
	delete [] newData;
}
