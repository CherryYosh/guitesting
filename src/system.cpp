#include "system.h"

System::System( Engine *ptEngine ){
	engine = ptEngine;
	msgList = NULL;
}

System::~System(){
	MessageList *temp;

	while( msgList ){
		temp = msgList->next;

		delete msgList;

		msgList = temp;
	}
}

void System::Start(){
}

void System::ProcessMessages(){
	//we do nothing here..
}

void System::ReceiveMessage( int messageID, void *parameters ){
	MessageList *temp = new MessageList;
	temp->ID = messageID;
	temp->parameters = parameters;
	temp->next = NULL;

	msg_mutex.lock(); //TODO: find a better way, or atleast make sure lock() is the best way
	//check to see if there is one
	if( !msgList ){
		msgList = temp;
		msg_mutex.unlock();
		return;
	}
	
	//we loop to the bottom and add
	while( msgList->next )
		msgList = msgList->next;

	msgList->next = temp;

	msg_mutex.unlock();
}
