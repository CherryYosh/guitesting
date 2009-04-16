#include <GL/glew.h>

#include "display.h"

#include "defines.h"
#include "engine.h"
#include "image.h"
#include "fontmgr.h"
#include "shader.h"
#include "timer.h"

Shader guiShader;
Shader textShader;

//TODO: clean up??
void LoadShaders(){
	guiShader.Load( "guiShader" );
	guiShader.GetUniformLoc( 0, "projection" );
	guiShader.GetUniformLoc( 1, "modelview" );
	guiShader.GetUniformLoc( 2, "tex0" );
	guiShader.GetAttributeLoc( 0, "vertex" );
	guiShader.GetAttributeLoc( 1, "tcoord" );
	
	textShader.Load( "textShader" );
	textShader.GetUniformLoc( 0, "projection" );
	textShader.GetUniformLoc( 1, "modelview" );
	textShader.GetUniformLoc( 2, "tex0" );
	textShader.GetAttributeLoc( 0, "vertex" );
	textShader.GetAttributeLoc( 1, "tcoord" );
	textShader.GetAttributeLoc( 2, "tcolor" );

	guiShader.Bind();
		glUniform1i( guiShader.uniform[2], 0 );
	guiShader.Unbind();
}

void SetupSDL(){
	SDL_EnableUNICODE(true);

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );

	SDL_WM_SetCaption( "Untitled Project", NULL );
}

Display::Display() : System(){
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0 ){
		printf( "Unable to init SDL: %s\n", SDL_GetError() );
		Engine::engine->Quit();
	}

	if( SDL_SetVideoMode( 640, 480, 0, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE ) == NULL ){
		printf( "Unable to init OpenGL: %s\n", SDL_GetError() );
		Engine::engine->Quit();
	}

	if( !InitImage() ){
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
	Mouse_Init();
	InitTimers();

	display = this;
}

Display::~Display(){
	delete camera;
	delete FPSTimer;
	delete gui;
	Mouse_Die();
	display = NULL;
}

void Display::InitTimers(){
	FPSTimer = new Timer();
	FPSTimer->SetFunction( boost::bind<void>(&Display::DrawFPS, this, FPSTimer->GetTicksPtr() ) );
	FPSTimer->SetRuntime( 5000 );
	FPSTimer->Start();
	
	timers.AddTimer( FPSTimer, true, false );
}

void Display::DrawFPS(unsigned int* data){
	printf( "FPS: %f\n", *data/5.0 );
	FPSTimer->Start();
}

void Display::Start(){
	timers.Update();

	Render();
}

void Display::Render(){	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gui->Render( &guiShader );
	gui->RenderText( &textShader );

	SDL_GL_SwapBuffers();
}

void Display::Resize( unsigned int width, unsigned int height ){	
	glViewport( 0, 0, width, height );
	camera->SetProjection( camera->fov, width / height, camera->zNear, camera->zFar );
}

void Display::OnMouseButtonChange(){
	Mouse_SetButtonState();
	
	if( Mouse_GetButtonState( 0 ) )
		gui->HitTest( Mouse_GetX(), Mouse_GetY() );
}

void Display::OnMouseMotion(){
	Mouse_SetPosition();

	if( Mouse_GetButtonState(0) ){ //dragging
		gui->Move( Mouse_GetChangeX(), Mouse_GetChangeY() );
	}
}

void Display::OnKeyPress( SDL_keysym sym ){
	gui->HandelKeyPress( sym.unicode );
}
