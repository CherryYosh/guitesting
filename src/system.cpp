#include "system.h"

System::System(){
	Messages = NULL;
	running = false;
}

System::~System(){
	running = false;
	Messages->clear();
	delete Messages;
}

void System::Start(){
}

void System::ProcessMessages(){
	//we do nothing here..
}

void System::ReceiveMessage( int messageID, void *parameters ){
	SYS_Message* msg = new SYS_Message;
	msg->id = messageID;
	msg->parameters = parameters;

	
	msg_mutex.lock(); 
	
	if( Messages == NULL )
		Messages = new std::vector<SYS_Message*>;
	
	Messages[0].push_back( msg );

	msg_mutex.unlock();
}
