#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef _DEBUG_
#include <stdio.h>
#endif

#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

#include "defines.h"

struct MessageList{
	int ID; //the id of the message (used to find what fuinction to call
	void *parameters; //options to use..
	MessageList *next;
};
//#include "engine.h"
class Engine; //circular..

class System{
public:
	System( Engine *ptEngine );
	virtual ~System();

	virtual void Start();
	virtual void ProcessMessages();
	virtual void ReceiveMessage( int messageID, void *parameters );
protected:
	bool running;
	Engine *engine;
	boost::shared_mutex msg_mutex;
	MessageList* msgList;
private:
};

#endif
