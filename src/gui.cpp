#ifdef _DEBUG_
#include <stdio.h>
#endif

#include "gui.h"
#include "thememgr.h"
#include "engine.h"

//TEMP DATA!!!
GLuint vbo[2];

Textbox* test;

GUI::GUI( Engine* ptEngine ) : System( ptEngine ){
	activeControl = NULL;
	isRecevingInput = false;

	SetTheme( "themes/default.theme" );

	AddControl( new Button( "close", 0, 0 ) );
	AddControl( new Button( "close", 200, 200 ) );
	test = new Editbox("topbar", 50, 50 );
	AddControl( test );
}

GUI::~GUI(){
	controls.clear(); //does this delete them all??
}

void GUI::SetTheme( char* themePath ){
	//TODO: clean this up..

	unsigned int size = ThemeMgr_LoadTheme( themePath );
	ThemeMgr_ThemeDataT* theme = ThemeMgr_GetTheme();

	//unsigned int* texData = new unsigned int[ size * 8 ]; //need to change this to floats
	//unsigned int* texIndices = new unsigned int[ size * 4 ];
	float* vertexData = new float[ 2 + size * 14 ];
	unsigned short* indices = new unsigned short [ size * 4 ];
	
	//we always start at 0,0, move based on the modelview
	vertexData[0] = 0; 
	vertexData[1] = 0;

	//first pass, set up the init data...
	for( unsigned int i = 0; i < size; i++ ){
		unsigned int slot = 2 + i * 14; //this is used so much, figure this might speed it up
		
		///////////////
		//time for vertex
		float x = (theme->data[i]->x2 - theme->data[i]->x); 
		float y = (theme->data[i]->y2 - theme->data[i]->y);
		
		vertexData[slot+0] = x;
		vertexData[slot+1] = 0;

		vertexData[slot+2] = x;
		vertexData[slot+3] = y;

		vertexData[slot+4] = 0;
		vertexData[slot+5] = y; 
		
		//textrue data
		x = (float)(theme->data[i]->x) / (float)theme->width;
		//heigh - y, convertes it from image space, 0,0, being top left, to texture space , 0,0 being bottome left
		y =  (float)( theme->height - theme->data[i]->y ) / (float)theme->height;
		
		float x2 =  (float)theme->data[i]->x2 / (float)theme->width;
		float y2 = (float)( theme->height - theme->data[i]->y2 )/ (float)theme->height;
		
		vertexData[slot+6] = x;
		vertexData[slot+7] = y;

		vertexData[slot+8] = x2;
		vertexData[slot+9] = y;

		vertexData[slot+10] = x2;
		vertexData[slot+11] = y2;

		vertexData[slot+12] = x;
		vertexData[slot+13] = y2;


		//set up the indices for the vertex's
		indices[i*4+0] = 0; //always start at 0,0
		indices[i*4+1] = i+1; //x,0
		indices[i*4+2] = i+2; //x,y
		indices[i*4+3] = i+3;

		//now we add the data
		GUI_GUIDataT* temp = new GUI_GUIDataT;
		
		temp->type = theme->data[i]->type;
		temp->vertexOffset = slot*sizeof(float);
		temp->textureOffset = (slot+6)*sizeof(float);
		temp->width = (theme->data[i]->x2 - theme->data[i]->x);
		temp->height = (theme->data[i]->y2 - theme->data[i]->y);
		
		guiData.push_back( temp );
	}

	//time to gen the VBO's
	glGenBuffers( 1, &vbo[0] );
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
	glBufferData( GL_ARRAY_BUFFER, (2 * sizeof(float)) + size * 14 * sizeof( float ), vertexData, GL_STATIC_DRAW );

	glGenBuffers( 1, &vbo[1] );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, size * 4 * sizeof( unsigned short ), indices, GL_STATIC_DRAW );
	
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	//clean the data
	delete [] vertexData;
	delete [] indices;
}

void GUI::Render( Shader* shader ){
	
	//We bind the theme image
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, ThemeMgr_GetImage() );

	//and then set up the vertex array
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	
	//and tell it to send the texture data to a attribute...
	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );

	//and now we draw them all
	for( std::vector<Control*>::iterator it = controls.begin(); it != controls.end(); ++it ){
		((Control*)*it)->Render();
	}

	glDisableVertexAttribArray( shader->attribute[0]);
	glDisableVertexAttribArray( shader->attribute[1]);

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void GUI::RenderText( Shader* shader ){
	test->RenderText();
}

void GUI::AddControl( Control* control ){
	for( unsigned int i = 0; i < guiData.size(); i++ ){
		if( control->type == guiData[i]->type ){
			//Alright, so we have these here becouse they will not change
			//so we store them from the theme, and then set them when ever 
			//a object of that type is called..
			control->vertexOffset = guiData[i]->vertexOffset;
			control->textureOffset = guiData[i]->textureOffset;
			control->SetWidth( guiData[i]->width );
			control->SetHeight( guiData[i]->height );
			break;
		}
	}

	controls.push_back( control );
}

void GUI::HitTest( int x, int y ){
	for( std::vector<Control*>::iterator it = controls.begin(); it != controls.end(); ++it ){
		if(  ((Control*)*it)->HitTest( x, y ) ){
			activeControl = ((Control*)*it);
			
			if( !isRecevingInput && activeControl->HasAttrib( CTRL_INPUT )){
				isRecevingInput = true;
				std::string* data = new std::string( "typing" );
				engine->ReceiveMessage( SYSTEM_INPUT, INPUT_CHANGE_PROFILE, (void*)data ); 
			}

			return;
		}
	}

	if( isRecevingInput ){
		isRecevingInput = false;
		std::string* data = new std::string( "default" );
		engine->ReceiveMessage( SYSTEM_INPUT, INPUT_CHANGE_PROFILE, (void*)data );
	}

	activeControl = NULL;
}

void GUI::Move( int x, int y ){
	if( activeControl != NULL )
		activeControl->Move( x, y );
}

void GUI::HandelKeyPress( int key, int mod ){
	if( activeControl != NULL ){ //shouldnt be needed but here just incase
		activeControl->onKeyPress( key, mod );
	}
}
