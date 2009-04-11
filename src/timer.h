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

/*

#include "engine.h"
#include "system.h"
#include <SDL/SDL.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>

template< typename R >
class Timer{
public:
	Timer( System* );
	Timer( int );
	~Timer();

	bool Start();
	void Stop(bool);
	void Restart(bool);
	void Tick();
	void Step();

	void SetFunction( boost::function<R()> );
	void SetRuntime( unsigned int );

	//use with care
	void RunCommand();

	unsigned int* GetTicksPtr();
	unsigned int* GetStepsPtr();
	unsigned int GetTicks();
	unsigned int GetSteps();

private:
	void WaistTime();

	unsigned int CurTicks;
	unsigned int StopTicks;
	unsigned int RunTime;
	unsigned int Ticks;
	unsigned int Steps;

	bool Running;
	bool SendData;

	boost::function<R()> Function;

	boost::thread Thread;
	boost::mutex Mutex;

	//the systems to send the message too...
	System* Owner;
	int OwnerID;
};

//=============================================================

template< typename R >
Timer<R>::Timer( System* system ){
	Owner = system;
	Running = false;
	SendData = false;
	CurTicks = 0;
	StopTicks = 0;
	RunTime = 1000;
	Ticks = 0;
	Steps = 0;
	OwnerID = 0;
}

template< typename R >
Timer<R>::Timer( int id ){
	Owner = NULL;
	OwnerID = id;
	Running = false;
	SendData = false;
	CurTicks = 0;
	StopTicks = 0;
	RunTime = 1000;
	Ticks = 0;
	Steps = 0;
}

template< typename R >
Timer<R>::~Timer(){
	Thread.join();
}

template< typename R >
bool Timer<R>::Start(){
	//a check to make sure its not started
	Mutex.lock();
		if( Running )
			return false;
	Mutex.unlock();

	//set up the timer
	Running = true;
	CurTicks = SDL_GetTicks();
	StopTicks = CurTicks + RunTime;
	
	//zero out the data
	Ticks = 0;
	Steps = 0;

	//this line SHOULD prevent thread creation overhead.. not sure
	boost::thread(boost::bind( &Timer<R>::WaistTime, this )).swap(Thread);
	return true;
}

template< typename R >
void Timer<R>::WaistTime(){
	Mutex.lock();
	while( Running && CurTicks < StopTicks ){
		Mutex.unlock();
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
		Mutex.lock();
		CurTicks = SDL_GetTicks();
	}

	if( !Running && !SendData ){
		Mutex.unlock();
		return;
	}

	
	Mutex.unlock();
	/*
	if( Owner != NULL )
		Owner->ReceiveMessage( TIMER_DONE, (void*)this );
	else
		engine->ReceiveMessage( OwnerID, TIMER_DONE, (void*)this );
	*/
/*
	printf( "FPS: %i\n", Ticks / 5 );
	//WaistTime();
	//Running = false;
}

template< typename R >
void Timer<R>::Stop( bool data ){
	Mutex.lock();
		Running = false;
		SendData = data;
	Mutex.unlock();
}

template< typename R >
void Timer<R>::Restart( bool reset ){
	Mutex.lock();
		StopTicks = SDL_GetTicks() + RunTime;
		
		if( reset ){
			Ticks = 0;
			Steps = 0;
		}

	Mutex.unlock();
}

template< typename R >
void Timer<R>::Tick(){
	Mutex.lock();
		Ticks++;
	Mutex.unlock();
}

template< typename R >
void Timer<R>::Step(){
	Mutex.lock();
		Steps++;
	Mutex.unlock();
}

template< typename R >
void Timer<R>::SetFunction( boost::function<R()> function){
	Mutex.lock();
		Function = function;
	Mutex.unlock();
}

template< typename R >
void Timer<R>::RunCommand(){
	Function();
}

template< typename R >
void Timer<R>::SetRuntime( unsigned int time ){
	//NOTE: this doesnt need to be locked, not used by the timer thread
	RunTime = time;
}

template< typename R >
unsigned int* Timer<R>::GetTicksPtr(){
	return &Ticks;
}

template< typename R >
unsigned int* Timer<R>::GetStepsPtr(){
	return &Steps;
}

template< typename R >
unsigned int Timer<R>::GetTicks(){
	return Ticks;
}

template< typename R >
unsigned int Timer<R>::GetSteps(){
	return Steps;
}

*/
#endif
