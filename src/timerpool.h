#ifndef TIMERPOOL_H
#define TIMERPOLL_H

#include <vector>
#include "timer.h"

struct timerpool_Data{
	Timer* timer;
	bool tick; //do we tick on update
	bool step; //do we step on update
};

class timerpool{
public:
	timerpool();
	~timerpool();

	void AddTimer( Timer*, bool, bool );
	void Update();
private:
	std::vector< timerpool_Data > timers;
};

#endif
