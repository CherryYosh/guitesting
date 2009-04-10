#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <boost/thread/thread.hpp>

#include "defines.h"

class Display;
class Input;


class Engine{
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

	struct Engine_Message{
		short classID;
		int messageID;
		void* parameters;
	};

	std::vector<Engine_Message*>* Messages;

	Input *input;
	Display *display;
};

static Engine* engine;

#endif
