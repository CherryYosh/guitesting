#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef _DEBUG_
#include <stdio.h>
#endif

#include <vector>
#include <boost/thread/mutex.hpp>

#include "defines.h"

class Engine; 

struct SYS_Message{
	int id; //the id of the message (used to find what fuinction to call
	void* parameters; //options to use..
};

class System{
public:
	System(); 
	virtual ~System();

	virtual void Start();
	virtual void ProcessMessages();
	virtual void ReceiveMessage( int messageID, void *parameters );
protected:
	bool running;
	boost::mutex msg_mutex;
	std::vector<SYS_Message*>* Messages;
private:
};

#endif
