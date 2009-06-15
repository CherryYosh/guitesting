/* 
 * File:   oglRenderer.cpp
 * Author: brandon
 * 
 * Created on May 4, 2009, 12:06 AM
 */


#include "oglBase.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <SDL/SDL.h>

#include "devilImage.h"
#include "../../camera.h"

Camera* oglBase::camera;

oglBase::oglBase() { }

oglBase::~oglBase() { }

/**
 * Creates the opengl context, using SDL. This also inits glew.
 */
void oglBase::CreateContext() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		printf("Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (SDL_SetVideoMode(640, 480, 0, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE) == NULL) {
		printf("Unable to init OpenGL: %s\n", SDL_GetError());
		exit(2);
	}

	if (!devilImage::InitDevIL()) {
		printf("Unable to init Image libary, closing!\n");
		exit(3);
	}

	glewInit();
}

/**
 * Setup the extra data for the context.. Things such as depth size and
 * color size should go here..
 */
void oglBase::SetupContext() {
	SDL_EnableUNICODE(true);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

	SDL_WM_SetCaption("Untitled Project", NULL);

	glViewport(0, 0, 640, 480);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.4);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 1.0);
}

/**
 * prepairs the base for rendering
 */
void oglBase::BeginRender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * stops the rendering for the base
 */
void oglBase::EndRender() {
	SDL_GL_SwapBuffers();
}

/**
 * Sets the camera, must be called atleast once
 * @param cam a pointer to the new camera
 */
void oglBase::SetCamera(Camera* cam) {
	if (cam != NULL){
		oglBase::camera = cam;
	}
}

/**
 * retruns the camera, make sure to check if it is null
 * @returns a pointer to the camera
 */
Camera* oglBase::GetCamera() {
	return oglBase::camera;
}

/**
 * Set up the viewport, in this case using glViewport
 */
void oglBase::SetViewport(int x, int y, int width, int height) {
	glViewport(x, y, width, height);
}

/**
 * gets the viewport, remeber to free the value
 * @returns returns the viewport, in this case 'glGetIntegerv(GL_VIEWPORT, .. '
 */
int* oglBase::GetViewport() {
	int* ret = new int[4];
	glGetIntegerv(GL_VIEWPORT, ret);
	return ret;
}
