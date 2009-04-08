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
#include "system.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

class Timer{
public:
        Timer( System* );
	~Timer();

	void Start();
	void Stop();
	void Restart();
	void Tick();
	void Step();

	void SetFunction( void(*)(void*), void* );


private:
	void WaistTime();

	unsigned int CurTicks;
	unsigned int StopTicks;
	unsigned int RunTime;
	unsigned int Ticks; 
	unsigned int Steps;

	bool Running;
	void (*Function)(void*);
	void* Parameters;
	
	boost::thread Thread;
	boost::mutex Mutex;

	System* Owner;
};

#endif
