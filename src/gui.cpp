#ifdef _DEBUG_
#include <stdio.h>
#endif

#include "gui.h"
#include "thememgr.h"
#include "engine.h"

GUI::GUI() : System(){
	ActiveWindow = NULL;
	isRecevingInput = false;
	numIndices = 0;	

	Screen = new Camera();
	Screen->SetOrtho( 0, 640, 480, 0, 1, 20 );
	Screen->Move( 0, 0, -1 );
	//set up the control
	Control_Init( "themes/default.theme" );
}

GUI::~GUI(){
	Windows.clear();
	delete [] ActiveWindow;
	delete Screen;
}

void GUI::Render( Shader* shader ){
	shader->Bind();
	shader->SetProjection( Screen->GetOrtho() );
	shader->SetModelview( Screen->GetModelview() );


	//We bind the theme image
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, ThemeMgr_GetImage() );

	glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);


	glVertexAttribPointer( shader->attribute[0], 2, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), 0 );
	glVertexAttribPointer( shader->attribute[1], 2, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), (GLvoid*)(NULL + 2 * sizeof(float)) );

	//this draws our whole table! wooo :P
	glDrawArrays( GL_QUADS, 0, numIndices );	
	
	glDisableVertexAttribArray( shader->attribute[0]);
	glDisableVertexAttribArray( shader->attribute[1]);

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	shader->Unbind();
}

void GUI::RenderText( Shader* shader ){
//	test->RenderText();
}


void GUI::HitTest( int x, int y ){
	//this is a quick excape.. most clicks SHOULD happen on the active window
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
