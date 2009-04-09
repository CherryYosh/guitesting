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

bool Timer::Start(){
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

void Timer::WaistTime(){
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

	Timer_RetData* data = new Timer_RetData;
	data->Function = Function;
	data->Ticks = Ticks;
	data->Steps = Steps;

	Owner->ReceiveMessage( FUNCTION, (void*)data );
	Running = false;
	Mutex.unlock();
}

void Timer::Stop( bool data ){
	Mutex.lock();
		Running = false;
		SendData = data;
	Mutex.unlock();
}

void Timer::Restart( bool reset ){
	Mutex.lock();
		StopTicks = SDL_GetTicks() + RunTime;
		
		if( reset ){
			Ticks = 0;
			Steps = 0;
		}
	Mutex.unlock();
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

void Timer::SetFunction( void* blah, void* function, void* parameters, int num ){
	Mutex.lock();
		Function = function;
		Parameters = parameters;
		NumParameters = num;
	Mutex.unlock();
}

void Timer::SetRuntime( unsigned int time ){
	RunTime = time;
}
