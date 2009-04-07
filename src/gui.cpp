#ifdef _DEBUG_
#include <stdio.h>
#endif

#include "gui.h"
#include "thememgr.h"
#include "engine.h"

GLuint vbo;

GUI::GUI( Engine* ptEngine ) : System( ptEngine ){
	ActiveWindow = NULL;
	isRecevingInput = false;
	numIndices = 0;	
	glGenBuffers( 1, &vbo );

	//SetTheme( "themes/default.theme" );
	Control_Init( vbo, "themes/default.theme" );
}

GUI::~GUI(){
}
/*
void GUI::SetTheme( char* themePath ){
	unsigned int size = ThemeMgr_LoadTheme( themePath );
	ThemeMgr_ThemeDataT* theme = ThemeMgr_GetTheme();

	float* data = new float[ size * 8 ];
	
	for( unsigned int i = 0; i < size; i++ ){
		unsigned int slot = i * 8; //readablitly, and prevent redunact cal's
		
		//textrue data
		float x = (float)(theme->data[i]->x) / (float)theme->width;
		//heigh - y, convertes it from image space, 0,0, being top left, to texture space , 0,0 being bottome left
		float y =  (float)( theme->height - theme->data[i]->y ) / (float)theme->height;
		
		float x2 =  (float)theme->data[i]->x2 / (float)theme->width;
		float y2 = (float)( theme->height - theme->data[i]->y2 )/ (float)theme->height;
		
		data[slot+0] = x;
		data[slot+1] = y;

		data[slot+2] = x2;
		data[slot+3] = y;

		data[slot+4] = x2;
		data[slot+5] = y2;

		data[slot+6] = x;
		data[slot+7] = y2;

		//add the data so it can get looked up
		CTRL_GUIDataT* temp = new CTRL_GUIDataT;
		
		temp->type = theme->data[i]->type;
		temp->textureOffset = (slot)*sizeof(float);
		temp->width = theme->data[i]->x2 - theme->data[i]->x;
		temp->height = theme->data[i]->y2 - theme->data[i]->y;
		
		guiData.push_back( temp );
	}

	//time to gen the VBO's
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, size * 8 * sizeof( float ), NULL, GL_STATIC_DRAW );
	glBufferData( GL_ARRAY_BUFFER, size * 8 * sizeof( float ), data, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	//clean the data
	delete [] data;
}
*/
void GUI::Render( Shader* shader ){
	
	//We bind the theme image
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, ThemeMgr_GetImage() );

	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );

	glVertexAttribPointer( shader->attribute[0], 2, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), 0 );
	glVertexAttribPointer( shader->attribute[1], 2, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), (GLvoid*)(NULL + 2 * sizeof(float)) );

	glDrawArrays( GL_QUADS, 0, numIndices );	
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray( shader->attribute[0]);
	glDisableVertexAttribArray( shader->attribute[1]);

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void GUI::RenderText( Shader* shader ){
	//test->RenderText();
}

void GUI::AddControl( Control* control ){
}

void GUI::HitTest( int x, int y ){
	if( ActiveWindow != NULL && ActiveWindow->HitTest( x, y ) ){
		return;
	}

	size_t size = Windows.size();
	for( int i = 0; i < size; i++ ){
		if( Windows[i]->HitTest( x, y ) ){
			ActiveWindow = Windows[i];
			return;
		}	
	}

	ActiveWindow = NULL;
}

void GUI::Move( int x, int y ){
	if( ActiveWindow != NULL )
		ActiveWindow->Move( x, y );
}

void GUI::HandelKeyPress( unsigned short unicode ){ 
}

//TODO: Create a theme file for windows!!!

void GUI::CreateWindowConsole( float x, float y ){
	Window* window = new Window;

	Button* close = new Button( "close", 0, 0 );
	Rule* topbar = 	new Rule( "topbar", 0, 0 );
	Rule* lsidebar = new Rule( "sidebar", 0, 0 );
	Rule* rsidebar = new Rule( "sidebar", 0, 0 );
	Rule* bottombar = new Rule( "bottombar", 0, 0 );
	Textbox* textarea = new Textbox( "textarea", 0, 0 );
	Editbox* inputarea = new Editbox( "textinput", 0, 0 );

	//now to position them
	//NOTE: top bar is positioned..
	close->Move( topbar->GetWidth() - (close->GetWidth() * 1.5 ),  topbar->GetHeight() * 0.25 );
	lsidebar->Move( 0, topbar->GetHeight() );
	rsidebar->Move( topbar->GetWidth() - rsidebar->GetWidth(), topbar->GetHeight() );
	bottombar->Move( lsidebar->GetWidth(), topbar->GetHeight() + lsidebar->GetHeight() - bottombar->GetHeight() );
	textarea->Move( lsidebar->GetWidth(), topbar->GetHeight() );
	inputarea->Move( lsidebar->GetWidth(), topbar->GetHeight() + textarea->GetHeight() );

	//now add them all
	window->AddChild( topbar, WINDOW_BOTTOM, false );
	window->AddChild( bottombar, WINDOW_BOTTOM, false );
	window->AddChild( lsidebar, WINDOW_BOTTOM, false );
	window->AddChild( rsidebar, WINDOW_BOTTOM, false );
	window->AddChild( close, WINDOW_TOP, false );
	window->AddChild( textarea, WINDOW_TOP, false );
	window->AddChild( inputarea, WINDOW_TOP, true );

	numIndices += 4 * 7;
	
	//now we move it (and all its children) and make it build its vbo
	window->width = topbar->GetWidth();
	window->height = topbar->GetHeight() + lsidebar->GetHeight();
	window->Move( x, y );
	Windows.push_back( window );
}
