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

#include <GL/gl.h>
#include <GL/glu.h>

#include "gui.h"

#include "input.h"
#include "engine.h"
#include "display.h"
#include "fontmgr.h"
#include "thememgr.h"
#include "gui/controls.h"


GUI::GUI() : System(){
	MouseOverWindow = NULL;
	ActiveWindow = NULL;
	IsRecevingInput = false;

	//set up the control
	Control_Init( "themes/default.theme" );
}

GUI::~GUI(){
	Windows.clear();
	delete MouseOverWindow;
	delete ActiveWindow;
}


//TODO: there has to be a better way to do this rendering
void GUI::Render( Shader* shader ){
	shader->Bind();
	shader->SetProjection( display->GetCameraOrtho() );

	//We bind the theme image
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, ThemeMgr_GetImage() );

	glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);

	for( unsigned int i = 0; i < Windows.size(); i++ ){
		Windows[i]->StepAnimation();
		Windows[i]->Render( shader );
	}
	glDisableVertexAttribArray( shader->attribute[0]);
	glDisableVertexAttribArray( shader->attribute[1]);

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	shader->Unbind();
}

void GUI::RenderAnimation( Shader* shader ){
	shader->Bind();
	shader->SetProjection( display->GetCameraOrtho() );

	//We bind the theme image
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, ThemeMgr_GetImage() );

	glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);

	for( unsigned int i = 0; i < Windows.size(); i++ ){
		Windows[i]->RenderAnimation( shader );
	}
	glDisableVertexAttribArray( shader->attribute[0]);
	glDisableVertexAttribArray( shader->attribute[1]);

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	shader->Unbind();
}

void GUI::RenderText( Shader* shader ){
	shader->Bind();
	shader->SetProjection( display->GetCameraOrtho() );

	glActiveTexture( GL_TEXTURE0 );
	//TODO: fix this hard coding, so that only 1 font is loaded at a time
	glBindTexture( GL_TEXTURE_2D, FontMgr_GetImage( 0 ) );

	glBindBuffer( GL_ARRAY_BUFFER, Control::GUI_vbo );
	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);
	glEnableVertexAttribArray(shader->attribute[2]);

	size_t size = Windows.size();
	for( unsigned int i = 0; i < size; i++ ){
		Windows[i]->RenderText( shader->attribute[0], shader->attribute[1], shader->attribute[2] );
	}

	glDisableVertexAttribArray( shader->attribute[0]);
	glDisableVertexAttribArray( shader->attribute[1]);
	glDisableVertexAttribArray( shader->attribute[2]);

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	shader->Unbind();
}

//TODO: Change this!! active window is not what it used to be!
bool GUI::HitTest( float x, float y ){
	//this is a quick excape..
	if( MouseOverWindow != NULL && MouseOverWindow->HitTest( x, y, display->GetCameraOrtho() ) ){
		return true;
	}

	size_t size = Windows.size();
	for( unsigned int i = 0; i < size; i++ ){
		if( Windows[i]->HitTest( x, y, display->GetCameraOrtho() ) ){
			MouseOverWindow = Windows[i];
			return true;
		}
	}

	MouseOverWindow = NULL;
	return false;
}

void GUI::Move( int x, int y ){
	if( x == 0 && y == 0 )
		return;

	if( ActiveWindow != NULL )
		ActiveWindow->Move( x, y );
}

void GUI::OnKeyPress( unsigned short unicode ){
	if( ActiveWindow != NULL )
		ActiveWindow->OnKeyPress( unicode );
}

void GUI::OnMousePress( unsigned short button, int mx, int my ){
	if( MouseOverWindow != NULL ){
			ActiveWindow = MouseOverWindow;
			ActiveWindow->OnMousePress( button, mx, my );
	}
}

//TODO: allow for used made themes
void GUI::CreateWindowConsole( float x, float y ){
	Window* window = new Window;

	Button* close = new Button( window, "close", 0, 0 );
	Rule* topbar = 	new Rule( window, "topbar", 0, 0 );
	Rule* lsidebar = new Rule( window, "sidebar", 0, 0 );
	Rule* rsidebar = new Rule( window, "sidebar", 0, 0 );
	Rule* bottombar = new Rule( window, "bottombar", 0, 0 );
	Label* textarea = new Label( window, "textarea", 0, 0 );
	Editbox* inputarea = new Editbox( window, "textinput", 0, 0 );

	close->SetCallback( boost::bind<int>(&Window::Close, window ) );

	//now to position them
	//NOTE: top bar is positioned..
	close->Move( topbar->GetWidth() - (close->GetWidth() * 1.5 ),  topbar->GetHeight() * 0.25 );
	lsidebar->Move( 0, topbar->GetHeight() );
	rsidebar->Move( topbar->GetWidth() - rsidebar->GetWidth(), topbar->GetHeight() );
	bottombar->Move( lsidebar->GetWidth(), topbar->GetHeight() + lsidebar->GetHeight() - bottombar->GetHeight() );
	textarea->Move( lsidebar->GetWidth(), topbar->GetHeight() );
	inputarea->Move( lsidebar->GetWidth(), topbar->GetHeight() + textarea->GetHeight() );

	//now add them all
	window->AddChild( topbar, 	WINDOW_BOTTOM, 	false 	);
	window->AddChild( bottombar,WINDOW_BOTTOM, 	false 	);
	window->AddChild( lsidebar, WINDOW_BOTTOM, 	false 	);
	window->AddChild( rsidebar, WINDOW_BOTTOM, 	false 	);
	window->AddChild( close, 	WINDOW_TOP, 	false 	);
	window->AddChild( textarea, WINDOW_TOP, 	false 	);
	window->AddChild( inputarea,WINDOW_TOP,  	true	);

	//now we move it (and all its children) and make it build its vbo
	window->Width = topbar->GetWidth();
	window->Height = topbar->GetHeight() + lsidebar->GetHeight();
	window->Move( x, y );
	Windows.push_back( window );

	textarea->AddStringsToVBO();
	inputarea->AddStringsToVBO();
}
