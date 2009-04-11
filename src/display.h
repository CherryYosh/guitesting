//============================
// the display driver class
// handels all opengl and drawing code

#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "mouse.h" 
#include "timer.h"
#include "timerpool.h"
#include "system.h"
#include "camera.h"
#include "gui.h"

class Display : public System {
public:
	Display();
	~Display();

	void Start();

	void DrawFPS(unsigned int*);
	
	void Render();
	void Resize( unsigned int width, unsigned int height );

	void OnMouseButtonChange();
	void OnMouseMotion();
	void OnKeyPress( SDL_keysym );
protected:
private:
	void InitTimers();

	Camera *camera;
	Timer *FPSTimer;
	timerpool timers;
	GUI *gui;
};

#endif
