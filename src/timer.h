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


struct Timer_RetData{
	void* Function;
 	unsigned int Ticks;
	unsigned int Steps;

};

class Timer{
public:
        Timer( System* );
	~Timer();

	bool Start();
	void Stop(bool);
	void Restart(bool);
	void Tick();
	void Step();

	void SetFunction( void*, void*, void*, int );
	void SetRuntime( unsigned int );

private:
	void WaistTime();

	unsigned int CurTicks;
	unsigned int StopTicks;
	unsigned int RunTime;
	unsigned int Ticks; 
	unsigned int Steps;

	bool Running;
	bool SendData;

	void* Function;
	void* Parameters;
	int NumParameters;
	
	boost::thread Thread;
	boost::mutex Mutex;

	System* Owner;
};

#endif
