#ifndef ENGINE_H
#define ENGINE_H

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

#include "defines.h"
#include "input.h"
#include "display.h"

class Engine// : public System
{
public:
	Engine();
	~Engine();

	void Start();
	void Run();
	void Quit();

	void ReceiveMessage( short classID, int messageID, void* parameters );
	void ProcessMessages();
protected:
private:
	bool running;
	
	boost::mutex msg_mutex;
	
	boost::thread engine_thread;
	boost::thread input_thread;
	boost::thread gui_thread;

	struct eMessageList{
		short classID;
		int messageID;
		void* parameters;
		eMessageList *next;
	} *list;

	Input *input;
	Display *display;
};

#endif
