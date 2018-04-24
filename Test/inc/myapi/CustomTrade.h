#pragma once

#include "server/UdpServer.h"
#include "server/UdpClient.h"
#include "adapter/MdDataStruct.h"
#include <pthread.h> 

class CustomTrade {
private:
	UdpClient *client;
	UdpServer *server;
	pthread_t tid;

public:
	CustomTrade(int clientID, int serverID);

	~CustomTrade();

	static void ListenRequest(void*);

	// my api
	virtual void OrderInsert(WZInputOrderField &order) = 0;
	virtual void OrderWithdraw(WZInputOrderField &order) = 0;
	virtual void Wait() {
		while(1) {}
	}
};