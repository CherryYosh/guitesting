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
#include "control.h"
#include "../thememgr.h"


std::vector<CTRL_GUIDataT*> guiData;
GLuint Control::GUI_vbo;

//Sets up the guiData so the controls can be created corectly
//IN:
//	vbo = id of the vbo to send the texture data too
//	path = the name of the default theme to load
//OUT: none
void Control_Init( const char* path ){
	unsigned int size = ThemeMgr_LoadTheme( path );
        const ThemeMgr_ThemeDataT* theme = ThemeMgr_GetTheme();

        for( unsigned int i = 0; i < size; i++ ){
                //textrue data
                float x = (float)(theme->data[i]->x) / (float)theme->width;
                //heigh - y, convertes it from image space, 0,0, being top left, to texture space , 0,0 being bottome left
                float y =  (float)( theme->height - theme->data[i]->y ) / (float)theme->height;

                float x2 =  (float)theme->data[i]->x2 / (float)theme->width;
                float y2 = (float)( theme->height - theme->data[i]->y2 )/ (float)theme->height;

                //add the data so it can get looked up
                CTRL_GUIDataT* temp = new CTRL_GUIDataT;

                temp->type = theme->data[i]->type;
                temp->width = theme->data[i]->x2 - theme->data[i]->x;
                temp->height = theme->data[i]->y2 - theme->data[i]->y;

		//set up the uv
		temp->s = x;
		temp->s2 = x2;
		temp->t = y;
		temp->t2 = y2;

                guiData.push_back( temp );
        }

        //time to gen the VBO's data
	glGenBuffers( 1, &Control::GUI_vbo );
        glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
        glBufferData( GL_ARRAY_BUFFER, 0, 0, GL_STREAM_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

//This should only be called when a new theme has been loaded or when the control is initlized
void Control::GetControlData(){
	size_t size = guiData.size();
	for( unsigned int i = 0; i < size; i++ ){
		if( Type == guiData[i]->type ){
			Width = guiData[i]->width;
			Height = guiData[i]->height;

			s = guiData[i]->s;
			s2 = guiData[i]->s2;
			t = guiData[i]->t;
			t2 = guiData[i]->t2;

			return;
		}
	}
}

Control::Control( Window* p, std::string t, float ix, float iy ){
	isEnabled = true;
	hasFocus = false;
	Type = t;
	x = ix;
	y = iy;
	VertexOffset = 0;

	Attributes = 0;
	isAnimated = false;
	GetControlData();
	Parent = p;
}

Control::~Control(){
	//is there anything to do??
}

void Control::Activate(){
	//nothing needs to be done
}

bool Control::HitTest( int mouseX, int mouseY ){
	return false;
}

void Control::OnMousePress( unsigned short button, int mx, int my ){
	//called by input
}

void Control::OnMouseRelease( int button ){
	//called by input
}

void Control::OnKeyPress( unsigned short unicode ){
	//called by input
}

void Control::OnKeyRelease( int key, int mod ){
	//called by input
}

void Control::Move( float cx, float cy ){
	x += cx;
	y += cy;
}

void Control::SetEnabled( bool value ){
	isEnabled = value;
}

void Control::SetFocus( bool value ){
	hasFocus = value;
}

void Control::SetCallback( boost::function<int()> callback ){
	m_Callback = callback;
}

void Control::SetWidth( float w ){
	Width = w;
}

void Control::SetHeight( float h ){
	Height = h;
}

bool Control::HasAttrib( unsigned short a ){
	//will only return true if all the attributes are there
	return a == ( Attributes & a );
}

void Control::SetDepth( float d ){
	Depth = d;
}

float Control::GetWidth(){
	return Width;
}

float Control::GetHeight(){
	return Height;
}

float Control::GetDepth(){
	return Depth;
}

bool Control::IsAnimated(){
	return isAnimated;
}

void Control::SetColor(float r, float g, float b, float a ){
	Color[0] = r;
	Color[1] = g;
	Color[2] = b;
	Color[3] = a;
}

void Control::SetColor( nv::vec4<float> c ){
	Color = c;
}

void Control::AddColor( nv::vec4<float> c ){
	Color += c;
}

float* Control::GetColorv(){
	return Color._array;
}

void Control::SetAnimated( bool v ){
	isAnimated = true;
}

void Control::OnMouseEnter(){
}

void Control::OnMouseLeave(){
}
