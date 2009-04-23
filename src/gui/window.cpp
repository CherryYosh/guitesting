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
#include "controls.h"
#include <SDL/SDL.h>

Window::Window(){
	VertexPosition = 0;
	VertexLength = 0;
	VertexPositionIsSet = false;
	ReciveInput = false;
	ActiveChild = NULL;
	AnimationOrigin = nv::vec3<float>(0.0);
	Modelview.make_identity();
	Modelview._43 = -1.0; //z value

	Animate( TRANSLATEXY, 100, 500, 10000, LINEAR );
	Animate( ORIGIN, nv::vec3<float>( 10, 10, 0 ), 0, 0, LINEAR );
	Animate( ROTATEZ, 90.0, 50, 10000, LINEAR );
}

Window::~Window(){
	ActiveChild = NULL;
	Children.clear();
}

void Window::AddChild( Control *child, int depth, bool rebuild ){
	child->SetDepth( depth );
	Children.push_back( child );

	if( rebuild )
		RebuildVBO(); //rebuild the vbo too
}

void Window::Move( float xChange, float yChange ){
	x += xChange;
	y += yChange;

	Modelview._array[12] += xChange;
	Modelview._array[13] += yChange;
}

int Window::Close(){
	/*TODO
	 * Allow saving, so upon reopening the buttons do not need to be reloaded
	 * will need a GC to truly delete the window after x seconds..
	 */
	//delete this;
	return 0;
}

void Window::Render( Shader* shader ){

	shader->SetModelview( Modelview._array );
	glVertexAttribPointer( shader->attribute[0], 2, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), 0 );
	glVertexAttribPointer( shader->attribute[1], 2, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), (GLvoid*)(2 * sizeof(float)) );

	//this draws our whole table! wooo :P
	for( unsigned int i = 0; i < Children.size(); i++ )
		if( !Children[i]->IsAnimated() )
				glDrawArrays( GL_QUADS, 4*i, 4 );
}

void Window::RenderAnimation( Shader* shader ){
	shader->SetModelview( Modelview._array );
	glVertexAttribPointer( shader->attribute[0], 2, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), 0 );
	glVertexAttribPointer( shader->attribute[1], 2, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), (GLvoid*)(2 * sizeof(float)) );

	//this draws our whole table! wooo :P
	for( unsigned int i = 0; i < Children.size(); i++ )
		if( Children[i]->IsAnimated() ){
				glUniform4fv( shader->uniform[3], 1, Children[i]->GetColorv() );
				glDrawArrays( GL_QUADS, 4*i, 4 );
		}
}

void Window::RenderText( int v, int t, int c ){
	size_t size = Children.size();
	for( unsigned int i = 0; i < size; i++ ){
		if( Children[i]->HasAttrib( CTRL_INPUT )){
			((Label*)Children[i])->RenderText( v, t, c );
		}
	}
}

bool Window::HitTest( float mx, float my, float* p ){
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	//float tx,ty;

	Unproject( mx, my, p, viewport, &mx, &my );
	//mx = tx;
	//my = ty;

	//TODO: make this faster :P
	if( mx < Width &&
			my < Height ){
			//make the origin at the box
			//mx -= x;
			//my -= y;

			if( ActiveChild != NULL && ActiveChild->HitTest( mx, my ) ){
				return true;
			}

			if( ActiveChild != NULL )
				ActiveChild->OnMouseLeave();

		//now we test the controls
		size_t size = Children.size();
		//TODO: Fix this hack, used to make sure the top item is found first
		//assumes the top items where added last
		for( unsigned int i = size-1; i > 0; i-- ){
			if(Children[i] != ActiveChild && Children[i]->HitTest(mx, my)){
				ActiveChild = Children[i];
				ActiveChild->OnMouseEnter();
				ReciveInput = ActiveChild->HasAttrib( CTRL_INPUT );
				return true;
			}
		}
		ActiveChild = NULL;
		return true;
	}

	if( ActiveChild != NULL )
		ActiveChild->OnMouseLeave();

	ActiveChild = NULL;
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

		vx =  child->x;
		vx2 = ( child->x + child->GetWidth() );
		vy = child->y;
		vy2 = ( child->y + child->GetHeight() );

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
		glFlushMappedBufferRange( GL_ARRAY_BUFFER_ARB, VertexPosition, VertexLength );
		glUnmapBuffer( GL_ARRAY_BUFFER );
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

		if( size > (length + VertexPosition) ) //if its set we need to copy everything after it
			memcpy( &newData[ VertexPosition + length ], &ptr[ VertexPosition + VertexLength  ], size - (length + VertexPosition) );

		glUnmapBuffer( GL_ARRAY_BUFFER );
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

void Window::OnKeyPress( unsigned short key ){
	if( ActiveChild != NULL )
		ActiveChild->OnKeyPress( key );
}

void Window::OnMousePress( unsigned short button, int mx, int my ){
	if( ActiveChild != NULL ){
		ActiveChild->OnMousePress( button, mx, my );
	}
}

void Window::Animate( int type, float value, unsigned int start, unsigned int duration, int interpolation , Control* ptr ){
	Animate( type, nv::vec4<float>( value, 0, 0, 0), start, duration, interpolation, ptr );
}

void Window::Animate( int type, nv::vec2<float> value, unsigned int start, unsigned int duration, int interpolation, Control* ptr ){
	Animate( type, nv::vec4<float>( value.x, value.y, 0, 0), start, duration, interpolation, ptr );
}

void Window::Animate( int type, nv::vec3<float> value, unsigned int start, unsigned int duration, int interpolation, Control* ptr ){
	if( type == ORIGIN ){
		AnimationOrigin = value;
		return;
	}

	Animate( type, nv::vec4<float>( value.x, value.y, value.z, 0), start, duration, interpolation, ptr );
}

void Window::Animate( int type, nv::vec4<float> value, unsigned int start, unsigned int duration, int interpolation, Control* ptr ){
	unsigned int ticks = SDL_GetTicks();

	AnimationType ani;
	ani.Type = type;
	ani.Interpolation = interpolation;
	ani.Object = ptr;
	ani.Duration = duration;
	ani.StartTicks = ticks + start;
	ani.LastTicks = ticks + start;
	ani.EndTicks = ticks + start + duration;
	ani.data = value;

	Animations.push_back( ani );
}

void Window::StepAnimation(){
	if( Animations.size() == 0 )
		return;

	unsigned int ticks = SDL_GetTicks();
	unsigned int step = 0;
	float scale;

	std::list<AnimationType>::iterator it;
	std::vector< std::list<AnimationType>::iterator > toDel;
	nv::vec4<float> data;
	for( it = Animations.begin(); it != Animations.end(); it++ ){

			if( ticks > it->StartTicks ){
				if( ticks > it->EndTicks ){
					step = it->EndTicks - it->LastTicks;
					toDel.push_back( it );
				} else {
					step = ticks - it->LastTicks;
					it->LastTicks = ticks;
				}

				scale = float(step) / float(it->Duration);
				data = it->data * scale;

				//Translation
				switch( (it->Type & TRANSLATEXYZ) ){
					case 0:													break;
					case TRANSLATEX: 	Move( data.x, 0 );					break;
					case TRANSLATEY: 	Move( 0, data.x );					break;
					case TRANSLATEXY: 	Move( data.x, data.y ); 			break;
					default: break;
				}

				//rotation
				if( (it->Type & ROTATEZ )){
					Modelview.rotate( data.x, 0.0, 0.0, 1.0 );
				}
				if( (it->Type & ROTATEORGZ )){
									Modelview.rotateOrigin( data.x, 0.0, 0.0, 1.0, AnimationOrigin );
				}
				if( (it->Type & ROTATESCREENZ )){
									Modelview.rotateScreen( data.x, 0.0, 0.0, 1.0, AnimationOrigin );
				}

				//color
				if( (it->Type & RGBACHANNEL )){
					if( it->Object != NULL ){
						it->Object->AddColor( data );
					}
				}
			}

	}

	//TODO: check for bugs
	for( unsigned int i = 0; i < toDel.size(); i++ ){
		Animations.erase( toDel[i] );
	}
}

void Window::Unproject( float winx, float winy, float* p, int* view, float* ox, float* oy ){
	nv::vec4<float> in = nv::vec4<float>(	((winx - view[0]) * 2.0) / view[2] - 1.0,
											-(((winy - view[1]) * 2.0) / view[3] - 1.0),
											-1.0,
											1.0 );

	nv::matrix4<float> proj;
	proj.set_value( p );
	proj *= Modelview;

	nv::vec4<float> ret = inverse( proj ) * in;

	if( ret.w == 0.0 )
		return;

	*ox = ret.x / ret.w;
	*oy = ret.y / ret.w;
}
