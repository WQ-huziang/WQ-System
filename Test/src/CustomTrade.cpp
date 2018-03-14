#include "myapi/CustomTrade.h"
#include "adapter/MdDataStruct.h"
#include "adapter/CTPAdapter.h"

CustomTrade::CustomTrade(int clientID, int serverID) {
	client = new UdpClient(clientID);
	server = new UdpServer(serverID);
	
	// create thread
	if (pthread_create(&tid, NULL, &CustomTrade::ListenRequest, NULL) != 0) {
		perror("create thread error");
		exit(1);
	}
}

CustomTrade::~CustomTrade() {
	delete client;
	delete server;
}

void CustomTrade::ListenRequest(void *arg) {
	char buf[BUFSIZ + 1];
	WZL2OrderField order;
	while (1) {
		if (server->recvUdpPackage(buf)) {
			switch (buf[0]) {
				case CHAR_OrderInsert: {
					WZL2OrderField::fromString(&order, buf + 1);
					this->OrderInsert(order);
					break;
				}
				case CHAR_OrderWithdraw: {
					WZL2OrderField::fromString(&order, buf + 1);
					this->OrderWithdraw(order);
					break;
				}
				default: {}
			}
		}
	}
}