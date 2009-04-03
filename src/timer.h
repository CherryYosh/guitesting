/*********************
 * A simple FPS timer
 *
 * Known bugs:
 * 	fps not caling right
 *
 * James Brandon Stevenson @ 10/05/2008
 */

#ifndef TIMER_H
#define TIMER_H

#include <SDL/SDL.h>
#include <stdio.h>

//simple FPS timer
class Timer{
public:
        Timer();

        void Update();

private:
	int startTicks;
	int frames;
};

#endif
