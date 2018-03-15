#include "myapi/CustomTrade.h"
#include "adapter/MdDataStruct.h"
#include "adapter/CTPAdapter.h"

CustomTrade::CustomTrade(int clientID, int serverID) {
	client = new UdpClient(clientID);
	server = new UdpServer(serverID);
}

CustomTrade::~CustomTrade() {
	delete client;
	delete server;
}

void CustomTrade::ListenRequest(void *arg) {
	char buf[BUFSIZ + 1];
	WZInputOrderField order;
	while (1) {
		if (server->recvUdpPackage(buf)) {
			switch (buf[0]) {
				case CHAR_OrderInsert: {
					WZInputOrderField::fromString(order, buf + TRAN_FLAG_LEN);
					this->OrderInsert(order);
					break;
				}
				case CHAR_OrderWithdraw: {
					WZInputOrderField::fromString(order, buf + TRAN_FLAG_LEN);
					this->OrderWithdraw(order);
					break;
				}
				default: {}
			}
		}
	}
}