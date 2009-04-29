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
#include <GL/gl.h>
#include <GL/glu.h>
#include<boost/bind.hpp>

#include "display.h"

#include "image.h"
#include "mouse.h"
#include "engine.h"
#include "shader.h"
#include "fontmgr.h"

//TODO: clean up?
Shader textShader;
Shader guiAnimationShader;

//TODO: clean up??
void LoadShaders(){
	textShader.Load( "textShader" );
	textShader.GetUniformLoc( 0, "projection" );
	textShader.GetUniformLoc( 1, "modelview" );
	textShader.GetUniformLoc( 2, "tex0" );
	textShader.GetAttributeLoc( 0, "vertex" );
	textShader.GetAttributeLoc( 1, "tcoord" );
	textShader.GetAttributeLoc( 2, "tcolor" );

	guiAnimationShader.Load( "guiAnimation" );
	guiAnimationShader.GetUniformLoc( 0, "projection" );
	guiAnimationShader.GetUniformLoc( 1, "modelview" );
	guiAnimationShader.GetUniformLoc( 2, "tex0" );
	guiAnimationShader.GetAttributeLoc( 0, "vertex" );
	guiAnimationShader.GetAttributeLoc( 1, "tcoord" );
	guiAnimationShader.GetAttributeLoc( 2, "tcolor" );
}

void SetupSDL(){
	SDL_EnableUNICODE(true);

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
	SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );

	SDL_WM_SetCaption( "Untitled Project", NULL );
}

Display::Display() : System(){
	display = this;

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0 ){
		printf( "Unable to init SDL: %s\n", SDL_GetError() );
		Engine::engine->Quit();
	}

	if( SDL_SetVideoMode( 640, 480, 0, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE ) == NULL ){
		printf( "Unable to init OpenGL: %s\n", SDL_GetError() );
		Engine::engine->Quit();
	}

	if( !Image_Init() ){
		printf( "Unable to init Image libary, closing!\n" );
		Engine::engine->Quit();
	}

	SetupSDL();

	glewInit();

	//start up the fontmgr, thanks rj
	FontMgr_Init();
	FontMgr_LoadFont( 0, "/usr/share/fonts/corefonts/arial.ttf", 16 );

	//moved these here to prevent issues with DevIL, also insures the contex is made by the time we get here
	gui = new GUI();
	camera = new Camera();

	gui->CreateWindowConsole( 50, 50 );
	gui->CreateTW();

	glViewport( 0, 0, 640, 480 );
	camera->SetProjection( 45.0, 640.0/480.0, 1.0, 1000.0 );
	camera->SetOrtho( 0, 640, 480, 0, 1, 20 );
	camera->Move( 0, 0, -1 );

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc( GL_GREATER, 0.4 );
	glEnable( GL_ALPHA_TEST );

	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	LoadShaders();
	Mouse_Init(this);
	InitTimers();
}

Display::~Display(){
	delete camera;
	delete gui;
	Mouse_Die();
	display = NULL;
}

void Display::InitTimers(){
	Timer* FPSTimer = new Timer( 5000, true );
	FPSTimer->SetFunction( boost::bind<void>(&Display::DrawFPS, this, FPSTimer->GetTicksPtr() ) );
	FPSTimer->Start();

	timers.AddTimer( FPSTimer, true, false );
	timers.AddTimer( Mouse_GetTimer() );
}

void Display::DrawFPS(unsigned int* data){
	printf( "FPS: %f\n", (*data) * 0.2 );
}

void Display::Start(){
	timers.Update();

	Render();
}

void Display::Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//here we draw the gui
	gui->Render( &guiAnimationShader );
	//gui->RenderText( &textShader );

	SDL_GL_SwapBuffers();
}

void Display::Resize( unsigned int width, unsigned int height ){
	glViewport( 0, 0, width, height );
	camera->SetProjection( camera->fov, width / height, camera->zNear, camera->zFar );
	camera->SetOrtho( 0, width, height, 0, 1, 20 );
}

void Display::OnMouseClick( unsigned short* num, bool final ){
	if( !gui->OnMouseClick( *num, final ) ){
		Mouse_StopTimer();
	}
}

void Display::OnMouseButtonChange(){
	Mouse_SetButtonState();

	if( Mouse_GetButtonState( 0 ) )
		gui->OnMousePress( 0, Mouse_GetX(), Mouse_GetY() );
}

void Display::OnMouseMotion(){
	Mouse_SetPosition();

	if( Mouse_GetButtonState(0) ){ //dragging
		gui->Move( Mouse_GetChangeX(), Mouse_GetChangeY() );
	} else {
		gui->HitTest( (float)Mouse_GetX(), (float)Mouse_GetY() );
	}
}

void Display::OnKeyPress( SDL_keysym sym ){
	gui->OnKeyPress( sym.unicode );
}

float* Display::GetCameraProjection(){
	return camera->GetProjection();
}

float* Display::GetCameraOrtho(){
	return camera->GetOrtho();
}
