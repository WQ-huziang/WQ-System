#include "myapi/CustomMd.h"
#include "adapter/MdDataStruct.h"

CustomMd::CustomMd(int clientID, int serverID) {
	client = new UdpClient(clientID);
	server = new UdpServer(serverID);
}

CustomMd::~CustomMd() {
	delete client;
	delete server;
}

void CustomMd::OnGetMarketData(const WZMarketDataField &mddata) {
	WZMarketDataField::toString(client->buf, mddata);
	client->sendUdpPackage(sizeof(mddata));
}