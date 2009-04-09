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
#include <boost/function.hpp>

template<class R, class C>
class Timer{
public:
	Timer( System* );
	~Timer();

	bool Start();
	void Stop(bool);
	void Restart(bool);
	void Tick();
	void Step();

	void SetFunction( boost::function<R()> );//boost::function<R (C*,ARGS...) > );
	void SetRuntime( unsigned int );

	//use with care
	void RunCommand();

	unsigned int* GetTicksPtr();

private:
	void WaistTime();

	unsigned int CurTicks;
	unsigned int StopTicks;
	unsigned int RunTime;
	unsigned int Ticks; 
	unsigned int Steps;

	bool Running;
	bool SendData;

	boost::function<R()> Function;//boost::function<R (C*, ARGS...)> Function;

	unsigned int NumArgs;

	boost::thread Thread;
	boost::mutex Mutex;

	System* Owner;
};

//=============================================================

template<class R, class C>
Timer<R,C>::Timer( System* system){
	Owner = system;
	Running = false;
	CurTicks = 0;
	StopTicks = 0;
	RunTime = 1000;
	Ticks = 0;
	Steps = 0;
}

template<class R, class C>
Timer<R,C>::~Timer(){
}

template<class R, class C>
bool Timer<R,C>::Start(){
	//a check to make sure its not started
	Mutex.lock();
		if( Running )
			return false;
	Mutex.unlock();

	Running = true;
	CurTicks = SDL_GetTicks();
	StopTicks = CurTicks + RunTime;
	Ticks = 16;
	Steps = 0;

	boost::thread(boost::bind( &Timer::WaistTime, this )).swap(Thread);
	return true;
}

template<class R, class C>
void Timer<R,C>::WaistTime(){
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

	Owner->ReceiveMessage( TIMER_DONE, (void*)this );
	Running = false;
	Mutex.unlock();
}

template<class R, class C>
void Timer<R,C>::Stop( bool data ){
	Mutex.lock();
		Running = false;
		SendData = data;
	Mutex.unlock();
}

template<class R, class C>
void Timer<R,C>::Restart( bool reset ){
	Mutex.lock();
		StopTicks = SDL_GetTicks() + RunTime;
		
		if( reset ){
			Ticks = 0;
			Steps = 0;
		}
	Mutex.unlock();
}

template<class R, class C>
void Timer<R,C>::Tick(){
	Mutex.lock();
		Ticks++;
	Mutex.unlock();
}

template<class R, class C>
void Timer<R,C>::Step(){
	Mutex.lock();
		Steps++;
	Mutex.unlock();
}

template<class R, class C>
void Timer<R,C>::SetFunction( boost::function<R()> function){//boost::function<R (C*, ARGS...)> function ){
	Mutex.lock();
		Function = function;
	Mutex.unlock();
}

template<class R, class C>
void Timer<R,C>::RunCommand(){
	//NOTE:This should NEVER be called via the timer's thread
	Function();//std::forward<ARGS>(ArgsList)...);
}

template<class R, class C>
void Timer<R,C>::SetRuntime( unsigned int time ){
	RunTime = time;
}

template<class R, class C>
unsigned int* Timer<R,C>::GetTicksPtr(){
	return &Ticks;
}

#endif
