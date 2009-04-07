#include <GL/glew.h>

#include "display.h"

#include "defines.h"
#include "engine.h"

#include "image.h"
#include "fontmgr.h"
#include "shader.h"

Shader guiShader;
Shader textShader;
//Shader* Control::_Shader = NULL;
//Shader* Textbox::_TextShader = NULL;

Display::Display( Engine *ptEngine ) : System( ptEngine ){
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0 ){
		printf( "Unable to init SDL: %s\n", SDL_GetError() );
		engine->ReceiveMessage( SYSTEM_ENGINE, QUIT, NULL );
	}

	if( SDL_SetVideoMode( 640, 480, 0, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE ) == NULL ){
		printf( "Unable to init OpenGL: %s\n", SDL_GetError() );
		engine->ReceiveMessage( SYSTEM_ENGINE, QUIT, NULL );
	}

	if( !InitImage() ){
		printf( "Unable to init Image libary, closing!\n" );
		engine->ReceiveMessage( SYSTEM_ENGINE, QUIT, NULL );
	}

	SDL_EnableUNICODE(true);

	glewInit();

	//start up the fontmgr, thanks rj
	FontMgr_Init();
	FontMgr_LoadFont( 0, "/usr/share/fonts/corefonts/arial.ttf", 16 );
	
	//moved these here to prevent issues with DevIL, also insures the contex is made by the time we get here
	gui = new GUI( ptEngine );
	timer = new Timer();
	mouse = new Mouse();
	camera = new Camera();

	gui->CreateWindowConsole( 50, 50 );
	
	glViewport( 0, 0, 640, 480 );
	camera->SetProjection( 45.0, 640.0/480.0, 1.0, 1000.0 );
	camera->SetOrtho( 0, 640, 480, 0, 1, 20 ); 
	camera->Move( 0, 0, -1 );

	//set percision for os compatibility
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
	
	SDL_WM_SetCaption( "Untitled Project", NULL );
	
	//=============================== set up default shaders!
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
	
	//Control::_Shader = &guiShader;
	//Textbox::_TextShader = &textShader;

	guiShader.Bind();
	glUniform1i( guiShader.uniform[2], 0 );
	guiShader.Unbind();
}

Display::~Display(){
	//TODO
	delete gui;
	delete timer;
}

void Display::Start(){
	running = true;
	
	while( running ){
		timer->Update();

		Render();

		ProcessMessages();
	}
}

void Display::ProcessMessages(){
        msg_mutex.lock();

        MessageList *temp = msgList;
        msgList = NULL;

        msg_mutex.unlock();

        MessageList *next;

	while( temp ){
                next = temp->next;

                switch( temp->ID ){
			case QUIT:
                                running = false;
                                break;
                	case MOUSE_PRESS:
				HandleMousePress();
				break;
			case MOUSE_RELEASE:
				HandleMouseRelease();
				break;
			case MOUSE_MOTION:
				HandleMouseMotion();
				break;
			case WINDOW_RESIZE:
				Resize( ((int*)temp->parameters)[0], ((int*)temp->parameters)[1] );
				break;
			case CAMERA_MOVE:
				camera->Move( ((double*)temp->parameters)[0], ((double*)temp->parameters)[1], ((double*)temp->parameters)[2] );
				break;
			case INPUT_KEYPRESS:
				gui->HandelKeyPress( ((unsigned short*)temp->parameters)[0] );//, ((int*)temp->parameters)[1] );
				break;
			default:
#ifdef _DEBUG_
				printf( "ERROR: unknown event id (%i) recived.\n", temp->ID );
#endif
				break;
		}

                if( temp->parameters != NULL )
			delete [] temp->parameters;
		delete temp;
                temp = next;
        }

}

void Display::Render(){	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	guiShader.Bind();
	guiShader.SetProjection( camera->GetOrtho() );
	guiShader.SetModelview( camera->GetModelview() );

	gui->Render( &guiShader );

	guiShader.Unbind();
/*
	textShader.Bind();
	textShader.SetProjection( camera->GetOrtho() );
	textShader.SetModelview( camera->GetModelview() );

	gui->RenderText( &textShader );

	textShader.Unbind();
*/
	SDL_GL_SwapBuffers();
}

void Display::Resize( unsigned int width, unsigned int height ){	
	glViewport( 0, 0, width, height );
	camera->SetProjection( camera->fov, width / height, camera->zNear, camera->zFar );

	window.width = width;
	window.height = height;
}

void Display::HandleMousePress(){
	mouse->SetButtonState();
	
	gui->HitTest( mouse->GetX(), mouse->GetY() );
}

void Display::HandleMouseRelease(){
	mouse->SetButtonState();
}

void Display::HandleMouseMotion(){
	mouse->SetMousePosition();

	if( mouse->GetButtonState(0) ){
		gui->Move( mouse->GetChangeX(), mouse->GetChangeY() );
	}
}
