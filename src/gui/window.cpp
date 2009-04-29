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
#include "../gui.h"
#include <SDL/SDL.h>

struct WINDOW_VBOVertex{
	float x, y;
	float s, t;
	float r,g,b,a;
};

struct AnimationType{
	unsigned int Type;
	unsigned int Interpolation;
	unsigned int EndTicks;
	unsigned int StartTicks;
	unsigned int LastTicks;
	unsigned int Duration; //end - start
	Control* Object;
	nv::vec4<float> data;
};

Window::Window( GUI* p ){
	VertexPosition = 0;
	VertexLength = 0;
	ReciveInput = false;
	MouseOverChild = NULL;
	AnimationOrigin = nv::vec3<float>(0.0);
	Modelview.make_identity();
	Modelview._43 = -1.0; //z value
	Parent = p;
}

Window::~Window(){
	Parent->CloseWindow( this );
	MouseOverChild = NULL;
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
	delete this;
	return 0;
}

//TODO: Find a better way to do the rendering
void Window::Render( Shader* shader ){
	shader->SetModelview( Modelview._array );
	glVertexAttribPointer( shader->attribute[0], 2, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), 0 );
	glVertexAttribPointer( shader->attribute[1], 2, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), (GLvoid*)(2 * sizeof(float)) );
	glVertexAttribPointer( shader->attribute[2], 4, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), (GLvoid*)(4 * sizeof(float)) );

	size_t size = Children.size();
	glDrawArrays( GL_QUADS, 0, size*4 );
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
	Unproject( mx, my, p, &mx, &my );

	if( mx > 0.0 && my > 0.0 &&
		mx < Width && my < Height ){
		if( MouseOverChild != NULL ){
			if( MouseOverChild->HitTest( mx, my ) )
			    return true;
			else
			    MouseOverChild->OnMouseLeave();
		}

		//now we test the controls
		unsigned int i = Children.size();
		while( 0 < i ){
			i--;
			
			if( Children[i] != MouseOverChild && Children[i]->HitTest(mx, my) ){
				MouseOverChild = Children[i];
				MouseOverChild->OnMouseEnter();
				return true;
			}
		} 
		
		MouseOverChild = NULL;
		return true;
	}

	MouseOverChild = NULL;
	return false;
}

void Window::UpdateControl( Control* cont ){
	WINDOW_VBOVertex* data = new WINDOW_VBOVertex[ 4 ];

	float vx, vx2, vy, vy2; //the vertex values, prevent redundant calculations
	float vs, vs2, vt, vt2;
	float* c;

	vx =  cont->x;
	vx2 = ( cont->x + cont->GetWidth() );
	vy = cont->y;
	vy2 = ( cont->y + cont->GetHeight() );

	vs = cont->s;
	vs2 = cont->s2;
	vt = cont->t;
	vt2 = cont->t2;

	//get the color
	c = cont->GetColorv();

	data[0].x = vx;
	data[0].y = vy;
	data[0].s = vs;
	data[0].t = vt;
	data[0].r = c[0];
	data[0].g = c[1];
	data[0].b = c[2];
	data[0].a = c[3];

	data[1].x = vx2;
	data[1].y = vy;
	data[1].s = vs2;
	data[1].t = vt;
	data[1].r = c[0];
	data[1].g = c[1];
	data[1].b = c[2];
	data[1].a = c[3];

	//bottom right
	data[2].x = vx2;
	data[2].y = vy2;
	data[2].s = vs2;
	data[2].t = vt2;
	data[2].r = c[0];
	data[2].g = c[1];
	data[2].b = c[2];
	data[2].a = c[3];

	//bottom left
	data[3].x = vx;
	data[3].y = vy2;
	data[3].s = vs;
	data[3].t = vt2;
	data[3].r = c[0];
	data[3].g = c[1];
	data[3].b = c[2];
	data[3].a = c[3];

	Control::GUI_vbo->Bind();
	Control::GUI_vbo->SetData( VertexPosition + cont->VertexOffset, 4 * sizeof( WINDOW_VBOVertex ), data );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	delete [] data;
}

void Window::UpdateVBO(){
	//NOTE: This is a lot like rebuild, only the end changes..

	size_t numChildren = Children.size();
	WINDOW_VBOVertex* data = new WINDOW_VBOVertex[ numChildren * 4 ];

	Control* child;
	unsigned int slot;
	float vx, vx2, vy, vy2; //the vertex values, prevent redundant calculations
	float vs, vs2, vt, vt2;
	float* c;
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

		//get the color
		c = child->GetColorv();

		//top left
		data[slot+0].x = vx;
		data[slot+0].y = vy;
		data[slot+0].s = vs;
		data[slot+0].t = vt;
		data[slot+0].r = c[0];
		data[slot+0].g = c[1];
		data[slot+0].b = c[2];
		data[slot+0].a = c[3];

		//top right
		data[slot+1].x = vx2;
		data[slot+1].y = vy;
		data[slot+1].s = vs2;
		data[slot+1].t = vt;
		data[slot+1].r = c[0];
		data[slot+1].g = c[1];
		data[slot+1].b = c[2];
		data[slot+1].a = c[3];

		//bottom right
		data[slot+2].x = vx2;
		data[slot+2].y = vy2;
		data[slot+2].s = vs2;
		data[slot+2].t = vt2;
		data[slot+2].r = c[0];
		data[slot+2].g = c[1];
		data[slot+2].b = c[2];
		data[slot+2].a = c[3];

		//bottom left
		data[slot+3].x = vx;
		data[slot+3].y = vy2;
		data[slot+3].s = vs;
		data[slot+3].t = vt2;
		data[slot+3].r = c[0];
		data[slot+3].g = c[1];
		data[slot+3].b = c[2];
		data[slot+3].a = c[3];
	}

	Control::GUI_vbo->Bind();
	Control::GUI_vbo->SetData( VertexPosition, VertexLength, data );
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
	float* c;
	for( unsigned int i = 0; i < numChildren; i++ ){
                //NOTE: I am hoping this code will be made into SSE :]
                slot = i * 4;
                child = Children[i];
		child->VertexOffset = slot * sizeof( WINDOW_VBOVertex );

                vx = x + child->x;
                vx2 = x + ( child->x + child->GetWidth() );
                vy = y + child->y;
                vy2 = y + ( child->y + child->GetHeight() );

                vs = child->s;
                vs2 = child->s2;
                vt = child->t;
                vt2 = child->t2;

		c = child->GetColorv();

                //top left
                data[slot+0].x = vx;
                data[slot+0].y = vy;
                data[slot+0].s = vs;
                data[slot+0].t = vt;
		data[slot+0].r = c[0];
		data[slot+0].g = c[1];
		data[slot+0].b = c[2];
		data[slot+0].a = c[3];

		//top right
                data[slot+1].x = vx2;
                data[slot+1].y = vy;
                data[slot+1].s = vs2;
                data[slot+1].t = vt;
		data[slot+1].r = c[0];
		data[slot+1].g = c[1];
		data[slot+1].b = c[2];
		data[slot+1].a = c[3];

		//bottom right
                data[slot+2].x = vx2;
                data[slot+2].y = vy2;
                data[slot+2].s = vs2;
                data[slot+2].t = vt2;
		data[slot+2].r = c[0];
		data[slot+2].g = c[1];
		data[slot+2].b = c[2];
		data[slot+2].a = c[3];

		//bottom left
                data[slot+3].x = vx;
                data[slot+3].y = vy2;
                data[slot+3].s = vs;
                data[slot+3].t = vt2;
		data[slot+3].r = c[0];
		data[slot+3].g = c[1];
		data[slot+3].b = c[2];
		data[slot+3].a = c[3];
        }

	VertexLength = (numChildren * 4 * sizeof(WINDOW_VBOVertex));
	
	Control::GUI_vbo->Bind();
	Control::GUI_vbo->AddData( VertexLength, data, &VertexPosition );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	
	delete [] data;
}

void Window::OnKeyPress( unsigned short key ){
	if( ActiveChild != NULL )
		ActiveChild->OnKeyPress( key );
}

void Window::OnMousePress( unsigned short button, int mx, int my ){
	if( MouseOverChild != NULL ){
		ActiveChild = MouseOverChild;
		ActiveChild->OnMousePress( button, mx, my );
		ReciveInput = ActiveChild->HasAttrib( CTRL_INPUT );
	}
}

/**
 *
 */
bool Window::OnMouseClick( unsigned short num, bool final ){
	if( ActiveChild != NULL ){
		return ActiveChild->OnMouseClick( num, final );
	}

	return false;
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
	nv::vec4<float> data;
	float scale;
	bool del;

	std::list<AnimationType>::iterator it = Animations.begin();
	while( it != Animations.end() ){
		del = false;

		if( ticks > it->StartTicks ){
			if( ticks > it->EndTicks ){
				step = it->EndTicks - it->LastTicks;
				del = true;
			} else {
				step = ticks - it->LastTicks;
				it->LastTicks = ticks;
			}

			scale = float(step) / float(it->Duration);
			data = it->data * scale;

			//Translation
			switch( (it->Type & TRANSLATEXYZ) ){
				case 0:								break;
				case TRANSLATEX: 	Move( data.x, 0 );			break;
				case TRANSLATEY: 	Move( 0, data.x );			break;
				case TRANSLATEXY: 	Move( data.x, data.y ); 		break;
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
					UpdateControl( it->Object );
				}
			}
		}

		if( del ){
			it = Animations.erase( it );
		} else {
			it++;
		}
	}
}

//TODO: Change the way animations are handeld to make this cleaner
//and faster
void Window::RemoveAnimation( Control* c ){
	std::list<AnimationType>::iterator it;
	for( it = Animations.begin(); it != Animations.end(); it++ ){
		if( it->Object == c ){
			Animations.erase( it );
			return;
		}
	}
}

void Window::Unproject( float winx, float winy, float* p, float* ox, float* oy ){
	GLint view[4];
	glGetIntegerv(GL_VIEWPORT, view);

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
