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

#include <boost/function.hpp>

class Timer{
public:
	Timer();
	~Timer();

	bool Start();
	void Stop();
	void Restart(bool);
	void Tick();
	void Step();
	void Update();

	void SetFunction( boost::function<void()> );
	void SetRuntime( unsigned int );

	unsigned int* GetTicksPtr();
	unsigned int* GetStepsPtr();
	unsigned int GetTicks();
	unsigned int GetSteps();

private:
	void RunCommand();

	unsigned int CurTicks;
	unsigned int StopTicks;
	unsigned int RunTime;
	unsigned int Ticks;
	unsigned int Steps;

	bool Running;

	boost::function<void()> Function;
};
#endif
