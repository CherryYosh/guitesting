#include "timerpool.h"

timerpool::timerpool(){
}

timerpool::~timerpool(){
	timers.clear();
}

void timerpool::AddTimer( Timer* timer, bool tick = false, bool step = false ){
	timerpool_Data data;
	data.timer = timer;
	data.tick = tick;
	data.step = step;

	timers.push_back( data );
}

void timerpool::Update(){
	size_t size = timers.size();
	timerpool_Data data;
	for( unsigned int i = 0; i < size; i++ ){
		data = timers[i];

		if( data.timer == NULL )
			break;

		if( data.tick )
			data.timer->Tick();
		if( data.step )
			data.timer->Step();
		data.timer->Update();
	}
}
