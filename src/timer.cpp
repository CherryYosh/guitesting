#include "timer.h"

Timer::Timer(){
	Running = false;
	CurTicks = 0;
	StopTicks = 0;
	RunTime = 1000;
	Ticks = 0;
	Steps = 0;
}

Timer::~Timer(){
}

bool Timer::Start(){
	//a check to make sure its not started
	if( Running )
		return false;

	//set up the timer
	Running = true;
	CurTicks = SDL_GetTicks();
	StopTicks = CurTicks + RunTime;
	
	//zero out the data
	Ticks = 0;
	Steps = 0;

	return true;
}

void Timer::Stop(){
	Running = false;
}

void Timer::Restart( bool reset ){
	StopTicks = SDL_GetTicks() + RunTime;
	
	if( reset ){
		Ticks = 0;
		Steps = 0;
	}
}

void Timer::Update(){
	if( !Running )
		return;

	CurTicks = SDL_GetTicks();

	if( CurTicks >= StopTicks ){
		Running = false; //NOTE: This has to be aboce RunCommand, so the command can correctly restart if it needs to
		RunCommand();
	}
}

void Timer::Tick(){
	Ticks++;
}

void Timer::Step(){
	Steps++;
}

void Timer::SetFunction( boost::function<void()> function){
	Function = function;
}

void Timer::RunCommand(){
	Function();
}

void Timer::SetRuntime( unsigned int time ){
	//NOTE: this doesnt need to be locked, not used by the timer thread
	RunTime = time;
}

unsigned int* Timer::GetTicksPtr(){
	return &Ticks;
}

unsigned int* Timer::GetStepsPtr(){
	return &Steps;
}

unsigned int Timer::GetTicks(){
	return Ticks;
}

unsigned int Timer::GetSteps(){
	return Steps;
}
