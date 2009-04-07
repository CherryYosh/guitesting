#include "control.h"
#include "../thememgr.h"


std::vector<CTRL_GUIDataT*> guiData;
GLuint Control::GUI_vbo;

//Sets up the guiData so the controls can be created corectly
//IN: 
//	vbo = id of the vbo to send the texture data too
//	path = the name of the default theme to load
//OUT: none
void Control_Init( GLuint vbo, const char* path ){
	unsigned int size = ThemeMgr_LoadTheme( path );
        ThemeMgr_ThemeDataT* theme = ThemeMgr_GetTheme();
                
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

 	vbo = Control::GUI_vbo; 
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

Control::Control( std::string t, float ix, float iy ){
	isEnabled = true;
	hasFocus = false;
	Type = t;
	x = ix;
	y = iy;
	VertexOffset = 0;

	Attributes = 0;
	GetControlData();
}

Control::~Control(){
	//is there anything to do??
}

void Control::Activate(){
	//nothing needs to be done
}

void Control::Render(){
	//NOTE: This is left over, no reasion to be called
}

bool Control::HitTest( int mouseX, int mouseY ){
	return false;
}

void Control::onMousePress( int button ){
	//called by input
}

void Control::onMouseRelease( int button ){
	//called by input
}

void Control::onKeyPress( unsigned short unicode ){ 
	//called by input
}

void Control::onKeyRelease( int key, int mod ){
	//called by input
}

void Control::Move( float cx, float cy ){

	x += cx;
	y += cy;

	//NOTE: Move will be batched, this is just for data..	
/*
	float* data = new float[8];
	data[0] = x;
	data[1] = y;

	data[2] = x + width;
	data[3] = y;

	data[4] = x + width;
	data[5] = y + height;

	data[6] = x;
	data[7] = y + height;

	glBindBuffer( GL_ARRAY_BUFFER, GUIBO );
	
	glBufferSubData( GL_ARRAY_BUFFER, vertexOffset, 8 * sizeof(float), data );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	delete [] data;
*/
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
