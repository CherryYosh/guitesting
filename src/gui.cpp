#ifdef _DEBUG_
#include <stdio.h>
#endif

#include "gui.h"
#include "thememgr.h"
#include "engine.h"
#include "input.h"
#include "fontmgr.h"

Editbox* test;

GUI::GUI() : System(){
	ActiveWindow = NULL;
	IsRecevingInput = false;
	numIndices = 0;	

	Screen = new Camera();
	Screen->SetOrtho( 0, 640, 480, 0, 1, 20 );
	Screen->Move( 0, 0, -1 );
	//set up the control
	Control_Init( "themes/default.theme" );
}

GUI::~GUI(){
	Windows.clear();
	delete ActiveWindow;
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
	glVertexAttribPointer( shader->attribute[1], 2, GL_FLOAT, GL_FALSE, sizeof(WINDOW_VBOVertex), (GLvoid*)(2 * sizeof(float)) );

	//this draws our whole table! wooo :P
	glDrawArrays( GL_QUADS, 0, numIndices );	
	
	glDisableVertexAttribArray( shader->attribute[0]);
	glDisableVertexAttribArray( shader->attribute[1]);

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	shader->Unbind();
}

void GUI::RenderText( Shader* shader ){
	shader->Bind();
	shader->SetProjection( Screen->GetOrtho() );
	shader->SetModelview( Screen->GetModelview() );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, FontMgr_GetImage( 0 ) );

	glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);
	glEnableVertexAttribArray(shader->attribute[2]);

	//for( unsigned int i = 0; i < Windows.size(); i++ ){
	//	if( Windows[i]->HasAttrib( CTRL_INPUT )){
	//		printf( "drawing text %i\n", i );
	//		((Label*)Windows[i])->RenderText( shader->attribute[0], shader->attribute[1] );
	//	}
	//}
	
	size_t size = Windows.size();
	for( unsigned int i = 0; i < size; i++ ){
		Windows[i]->RenderText( shader->attribute[0], shader->attribute[1], shader->attribute[2] );
	}
/*	
	glVertexAttribPointer( shader->attribute[0], 2, GL_FLOAT, GL_FALSE, 32, (GLvoid*)(numIndices*sizeof(float)) );
	glVertexAttribPointer( shader->attribute[1], 2, GL_FLOAT, GL_FALSE, 32, (GLvoid*)(numIndices*sizeof(float) + 2*sizeof(float)) );
	glVertexAttribPointer( shader->attribute[2], 4, GL_FLOAT, GL_FALSE, 32, (GLvoid*)(numIndices*sizeof(float) + 4*sizeof(float)) );

	glDrawArrays( GL_QUADS, 0, 16 );
*/
	glDisableVertexAttribArray( shader->attribute[0]);
	glDisableVertexAttribArray( shader->attribute[1]);
	glDisableVertexAttribArray( shader->attribute[2]);

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	shader->Unbind();
}


bool GUI::HitTest( int x, int y ){
	//this is a quick excape.. most clicks SHOULD happen on the active window
	if( ActiveWindow != NULL && ActiveWindow->HitTest( x, y ) ){
		return true;
	}
	
	size_t size = Windows.size();
	for( unsigned int i = 0; i < size; i++ ){
		if( Windows[i]->HitTest( x, y ) ){
			ActiveWindow = Windows[i];
			
			if( IsRecevingInput != ActiveWindow->ReciveInput ){
				IsRecevingInput = !IsRecevingInput;
				input->SetProfile( IsRecevingInput ? "typing" : "default" );
			}

			return true;
		}	
	}

	if( IsRecevingInput ){
		input->SetProfile( "default" );
		IsRecevingInput = false;
	}

	ActiveWindow = NULL;
	return false;
}

void GUI::Move( int x, int y ){
	if( ActiveWindow != NULL )
		ActiveWindow->Move( x, y );
}

void GUI::HandelKeyPress( unsigned short unicode ){ 
	if( ActiveWindow != NULL )
		ActiveWindow->OnKeyPress( unicode );
}

void GUI::CreateWindowConsole( float x, float y ){
	Window* window = new Window;

	Button* close = new Button( "close", 0, 0 );
	Rule* topbar = 	new Rule( "topbar", 0, 0 );
	Rule* lsidebar = new Rule( "sidebar", 0, 0 );
	Rule* rsidebar = new Rule( "sidebar", 0, 0 );
	Rule* bottombar = new Rule( "bottombar", 0, 0 );
	Label* textarea = new Label( "textarea", 0, 0 );
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
	window->AddChild( close, WINDOW_TOP, true );
	window->AddChild( textarea, WINDOW_TOP, false );
	window->AddChild( inputarea, WINDOW_TOP, true );

	test = inputarea;

	numIndices += 4 * 7;
	
	//now we move it (and all its children) and make it build its vbo
	window->width = topbar->GetWidth();
	window->height = topbar->GetHeight() + lsidebar->GetHeight();
	window->Move( x, y );
	Windows.push_back( window );
	
	textarea->AddStringsToVBO();
	inputarea->AddStringsToVBO();
}
