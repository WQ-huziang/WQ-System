#pragma once

#include "server/UdpServer.h"
#include "server/UdpClient.h"
#include "adapter/MdDataStruct.h" 

class CustomTrade {
private:
	UdpClient *client;
	UdpServer *server;

public:
	CustomTrade(int clientID, int serverID);

	~CustomTrade();

	void ListenRequest(void*);

	// my api
	virtual void OrderInsert(WZInputOrderField &order) = 0;
	virtual void OrderWithdraw(WZInputOrderField &order) = 0;
	virtual void Wait() {
		while(1) {}
	}
};