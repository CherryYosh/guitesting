#include "timer.h"

Timer::Timer( System* system ){
	Owner = system;
	Running = false;
	CurTicks = 0;
	StopTicks = 0;
	RunTime = 1000;
	Ticks = 0;
	Steps = 0;

	Function = NULL;
	Parameters = NULL;
}

Timer::~Timer(){
}

void Timer::Start(){	
	Running = true;
	CurTicks = SDL_GetTicks();
	StopTicks = CurTicks + RunTime;

	boost::thread(boost::bind( &Timer::WaistTime, this )).swap(Thread);
}

void Timer::WaistTime(){
	while( Running && CurTicks < StopTicks ){
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
		CurTicks = SDL_GetTicks();
	}

	//temp
	printf("2 %p\n", (void*)*Function );
	Owner->ReceiveMessage( FUNCTION, (void*)*Function );
	Running = false;
}

void Timer::Stop(){
	Mutex.lock();
		Running = false;
		//SendData = data;
	Mutex.unlock();
}

void Timer::Restart(){
}

void Timer::Tick(){
	Mutex.lock();
		Ticks++;
	Mutex.unlock();
}

void Timer::Step(){
	Mutex.lock();
		Steps++;
	Mutex.unlock();
}

void Timer::SetFunction( void function(void*), void* parameters ){
	Mutex.lock();
		Function = function;
		printf( "3 %p\n", function );
		Parameters = parameters;
	Mutex.unlock();
}
