#include <GL/glew.h>

#include "display.h"

#include "defines.h"
#include "engine.h"

#include "image.h"
#include "fontmgr.h"
#include "shader.h"

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

void Blah( void* data ){
	printf( "here!\n" );
}

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

	SetupSDL();
	
	glewInit();

	//start up the fontmgr, thanks rj
	FontMgr_Init();
	FontMgr_LoadFont( 0, "/usr/share/fonts/corefonts/arial.ttf", 16 );
	
	//moved these here to prevent issues with DevIL, also insures the contex is made by the time we get here
	gui = new GUI( ptEngine );
	timer = new Timer(this);
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
}

Display::~Display(){
	//TODO
	delete gui;
	delete timer;
	delete camera;
	Mouse_Die();
}

void (*ptr)(void*);

void Display::Start(){
	running = true;
	unsigned char MouseUpdate;

	timer->SetFunction( &Blah, NULL );
	printf( "1 %p\n", &Blah );
	ptr = &Blah;
	ptr(NULL);
	timer->Start();
	
	while( running ){
		Render();

		ProcessMessages();
		
		if( (MouseUpdate = Mouse_SetState()) != 0 ){
			if( MouseUpdate & 0x01 ) OnMouseButtonChange();
			if( MouseUpdate & 0x02 ) OnMouseMotion();
		}
	
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
			case MOUSE_RELEASE:
				OnMouseButtonChange();
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
			case FUNCTION:
				//FEAR ME!!!! MUAHAHAHAHAAHAH
				//((void(*)(void*))temp->parameters) -- gets a pointer to a function
				//(void*)((char*)temp->parameters)[sizeof(void(*)(void*))-1] -- passes in the data
				((void(*)(void*))temp->parameters)((void*)((char*)temp->parameters)[sizeof(void(*)(void*))-1]);
				break;
			default:
#ifdef _DEBUG_
				printf( "ERROR: unknown event id (%i) recived.\n", temp->ID );
#endif
				break;
		}

//                if( temp->parameters != NULL )
//			delete [] temp->parameters;
		delete temp;
                temp = next;
        }

}

void Display::Render(){	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gui->Render( &guiShader );

	SDL_GL_SwapBuffers();
}

void Display::Resize( unsigned int width, unsigned int height ){	
	glViewport( 0, 0, width, height );
	camera->SetProjection( camera->fov, width / height, camera->zNear, camera->zFar );
}

void Display::OnMouseButtonChange(){
	if( Mouse_GetButtonState( 0 ) )
		gui->HitTest( Mouse_GetX(), Mouse_GetY() );
}

void Display::OnMouseMotion(){
	if( Mouse_GetButtonState(0) ){ //dragging
		gui->Move( Mouse_GetChangeX(), Mouse_GetChangeY() );
	}
}
